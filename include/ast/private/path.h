#pragma once

#include <stdint.h>
#include "ast/api/expr.h"
#include "ast/api/type.h"
#include "core/location.h"
#include "core/slice.h"
#include "sema/module/behaviour/table/api.h"
#include "sema/module/behaviour/table/path.h"
#include "sema/module/decls/decls.h"
#include "sema/value.h"
#include "sema/module/decls.h"
#include "sema/module.h"
#include "sema/type.h"
#include "sema/value.h"
#include "sema/value/value.h"

typedef enum {
	AST_PATH_SEG_IDENT,
	AST_PATH_SEG_DEREF,
	AST_PATH_SEG_SIZEOF,
	AST_PATH_SEG_NULL,
    AST_PATH_SEG_GENERIC,
} AstPathSegmentType;

typedef enum {
	SEMA_PATH_DEREF,
    SEMA_PATH_ARRAY_LEN,
    SEMA_PATH_SLICE_LEN,
    SEMA_PATH_SLICE_RAW,
    SEMA_PATH_STRUCT_MEMBER,
    SEMA_PATH_IS_NULL,
    SEMA_PATH_DECL,
    SEMA_PATH_EXT_FUNC_DIRECT,
    SEMA_PATH_EXT_FUNC_REF,
    SEMA_PATH_BUILD_GENERIC,
    SEMA_PATH_BTABLE_PATH,
    SEMA_PATH_SIZEOF
} SemaPathType;

typedef struct {
    size_t idx;
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
        struct {
            SemaBehaviourTable *table;
        } generic;
        SemaType *deref_type;
        SemaType *slice_type;
        SemaType *optional_type;
        SemaDecl *decl;
        SemaDecl *ext_func_decl;
		size_t array_length;
        SemaPathStructMember struct_member;
        SemaBehaviourTablePath btable_path;
    };
} SemaPath;

typedef struct {
	AstPathSegmentType type;
    FileLocation loc;
	union {
		Slice ident;
        AstType **generic_params;
	};
    SemaPath sema;
} AstPathSegment;

typedef struct AstPath {
	AstPathSegment *segments;
} AstPath;
