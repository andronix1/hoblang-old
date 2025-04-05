#pragma once

#include "ast/interface/type.h"
#include "core/location.h"
#include "core/slice/slice.h"
#include "sema/interface/decl.h"
#include "sema/interface/value.h"
#include "sema/nodes/shared/path_seg_kind.h"
#include <llvm-c/Types.h>
#include <malloc.h>

typedef enum {
    AST_PATH_SEGMENT_IDENT,
    AST_PATH_SEGMENT_DEREF,
    AST_PATH_SEGMENT_SIZEOF,
    AST_PATH_SEGMENT_NULL,
    AST_PATH_SEGMENT_GENERIC,
} AstPathSegmentKind;

typedef struct {
    AstType **params;
} AstPathSegmentGeneric;

typedef struct {
    AstPathSegmentKind kind;
    FileLocation loc;
    union {
        Slice ident;
        AstPathSegmentGeneric generic;
    };
    struct {
        SemaPathSegKind kind;
        SemaValue *from_value;
        union {
            size_t struct_idx;
            SemaDecl *decl;
            SemaDecl *ext_func_decl;
        };
    } sema;
} AstPathSegment;

typedef struct AstPath {
    AstPathSegment *segments;

    struct {
        LLVMValueRef ext_handle;
    } llvm;
} AstPath;

static inline AstPathSegment ast_path_segment_new_generic(FileLocation loc, AstType **params) {
    AstPathSegment result = {
        .kind = AST_PATH_SEGMENT_GENERIC,
        .loc = loc,
        .generic = {
            .params = params
        }
    };
    return result;
}

static inline AstPathSegment ast_path_segment_new_null(FileLocation loc) {
    AstPathSegment result = { .kind = AST_PATH_SEGMENT_NULL, .loc = loc };
    return result;
}

static inline AstPathSegment ast_path_segment_new_sizeof(FileLocation loc) {
    AstPathSegment result = { .kind = AST_PATH_SEGMENT_SIZEOF, .loc = loc };
    return result;
}

static inline AstPathSegment ast_path_segment_new_deref(FileLocation loc) {
    AstPathSegment result = { .kind = AST_PATH_SEGMENT_DEREF, .loc = loc };
    return result;
}

static inline AstPathSegment ast_path_segment_new_ident(FileLocation loc, Slice ident) {
    AstPathSegment result = {
        .kind = AST_PATH_SEGMENT_IDENT,
        .loc = loc,
        .ident = ident
    };
    return result;
}

static inline AstPath *ast_path_new(AstPathSegment *segments) {
    AstPath *result = malloc(sizeof(AstPath));
    result->segments = segments;
    return result;
}
