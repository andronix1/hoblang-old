#pragma once

#include "ast/interface/path.h"
#include "ast/interface/type.h"
#include "ast/interface/expr.h"
#include "sema/interface/type.h"
#include <malloc.h>

typedef enum {
	AST_TYPE_PATH,
	AST_TYPE_FUNC,
	AST_TYPE_SLICE,
	AST_TYPE_ARRAY,
	AST_TYPE_OPTIONAL,
	AST_TYPE_POINTER
} AstTypeKind;

typedef struct {
    AstType *of;
    AstExpr *length;
} AstTypeArray;

typedef struct {
    AstType **args;
    AstType *returns;
} AstTypeFunc;

typedef struct AstType {
    AstTypeKind kind;
    union {
        AstPath *path;
        AstTypeFunc func;
        AstType *slice_of;
        AstTypeArray array;
        AstType *optional_of;
        AstType *pointer_to;
    };

    struct {
        SemaType *type;
    } sema;
} AstType;

static inline AstType *ast_type_new_array(AstType *of, AstExpr *length) {
    AstType *result = malloc(sizeof(AstType));
    result->kind = AST_TYPE_ARRAY;
    result->array.of = of;
    result->array.length = length;
    result->sema.type = NULL;
    return result;
}

static inline AstType *ast_type_new_pointer(AstType *to) {
    AstType *result = malloc(sizeof(AstType));
    result->kind = AST_TYPE_POINTER;
    result->pointer_to = to;
    result->sema.type = NULL;
    return result;
}

static inline AstType *ast_type_new_optional(AstType *of) {
    AstType *result = malloc(sizeof(AstType));
    result->kind = AST_TYPE_OPTIONAL;
    result->optional_of = of;
    result->sema.type = NULL;
    return result;
}

static inline AstType *ast_type_new_slice(AstType *of) {
    AstType *result = malloc(sizeof(AstType));
    result->kind = AST_TYPE_SLICE;
    result->slice_of = of;
    result->sema.type = NULL;
    return result;
}

static inline AstType *ast_type_new_func(AstType **args, AstType *returns) {
    AstType *result = malloc(sizeof(AstType));
    result->kind = AST_TYPE_FUNC;
    result->func.args = args;
    result->func.returns = returns;
    result->sema.type = NULL;
    return result;
}

static inline AstType *ast_type_new_path(AstPath *path) {
    AstType *result = malloc(sizeof(AstType));
    result->kind = AST_TYPE_PATH;
    result->path = path;
    result->sema.type = NULL;
    return result;
}
