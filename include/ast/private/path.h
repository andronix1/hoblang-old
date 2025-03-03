#pragma once

#include <stdint.h>
#include "core/location.h"
#include "core/slice.h"
#include "sema/module/decls/decls.h"
#include "sema/value.h"
#include "sema/module/decls.h"
#include "sema/module/parts/decls/struct/struct.h"
#include "sema/module.h"
#include "sema/type.h"
#include "sema/value.h"
#include "sema/value/value.h"

typedef enum {
	AST_PATH_SEG_IDENT,
	AST_PATH_SEG_DEREF,
	AST_PATH_SEG_SIZEOF,
	AST_PATH_SEG_NULL,
} AstPathSegmentType;

typedef enum {
	SEMA_PATH_DEREF,
    SEMA_PATH_ARRAY_LEN,
    SEMA_PATH_SLICE_LEN,
    SEMA_PATH_SLICE_RAW,
    SEMA_PATH_STRUCT_MEMBER,
    SEMA_PATH_IS_NULL,
    SEMA_PATH_DECL,
    SEMA_PATH_SIZEOF
} SemaPathType;

typedef struct {
    SemaStructMember *member;
    SemaType *of;
} SemaPathStructMember;

typedef struct {
    SemaPathType type;
    SemaValue *value;
    union {
        struct {
            SemaType *type;
            SemaType *output_type;
        } sizeof_op;
        SemaType *deref_type;
        SemaType *slice_type;
        SemaType *optional_type;
        SemaDecl *decl;
		size_t array_length;
        SemaPathStructMember struct_member;
    };
} SemaPath;

typedef struct {
	AstPathSegmentType type;
    FileLocation loc;
	union {
		Slice ident;
	};
    SemaPath sema;
} AstPathSegment;

typedef struct AstPath {
	AstPathSegment *segments;
} AstPath;
