#pragma once

#include <stdint.h>
#include "core/location.h"
#include "core/slice.h"
#include "sema/value.h"
#include "sema/module/decls.h"
#include "sema/module/parts/decls/struct/struct.h"
#include "sema/module.h"
#include "sema/type.h"
#include "sema/value.h"

typedef struct AstDeclPathSegment {
	Slice ident;
    FileLocation loc;
} AstDeclPathSegment;

typedef struct AstDeclPath {
	AstDeclPathSegment *segments;

	union {
		SemaModule *module;
		SemaScopeDecl *decl;
    	SemaType *type;
	};
} AstDeclPath;

typedef enum {
	AST_INNER_PATH_SEG_IDENT,
	AST_INNER_PATH_SEG_DEREF,
	AST_INNER_PATH_SEG_SIZEOF,
	AST_INNER_PATH_SEG_NULL,
} AstInnerPathSegmentType;

typedef enum {
	SEMA_INNER_PATH_DEREF,
    SEMA_INNER_PATH_ARRAY_LEN,
    SEMA_INNER_PATH_SLICE_LEN,
    SEMA_INNER_PATH_SLICE_RAW,
    SEMA_INNER_PATH_STRUCT_MEMBER,
    SEMA_INNER_PATH_IS_NULL,
    SEMA_INNER_PATH_SIZEOF
} SemaInnerPathType;

typedef struct {
    SemaStructMember *member;
    SemaType *of;
} SemaInnerPathStructMember;

typedef struct {
    SemaInnerPathType type;
    SemaValue *value;
    union {
        SemaType *sizeof_type;
        SemaType *deref_type;
        SemaType *slice_type;
        SemaType *optional_type;
		size_t array_length;
        SemaInnerPathStructMember struct_member;
    };
} SemaInnerPath;

typedef struct {
	AstInnerPathSegmentType type;
    FileLocation loc;
	union {
		Slice ident;
	};
    SemaInnerPath sema;
} AstInnerPathSegment;

typedef struct AstInnerPath {
	AstInnerPathSegment *segments;
} AstInnerPath;

typedef enum {
    AST_PATH_SEGMENT_IDENT
} AstPathSegmentType;

typedef struct AstPath {
	AstDeclPath decl_path;
	AstInnerPath inner_path;
} AstPath;
