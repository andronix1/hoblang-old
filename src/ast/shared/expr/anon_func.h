#pragma once

#include "ast/interface/type.h"
#include "ast/interface/body.h"
#include "core/slice/slice.h"

typedef struct {
    Slice name;
    AstType *type; // NULLABLE
} AstExprAnonFuncArg;

typedef struct {
    AstExprAnonFuncArg *args;
    AstType *returns; // NULLABLE
    AstBody *body;
} AstExprAnonFunc;

static inline AstExprAnonFuncArg ast_expr_anon_fun_arg_new(Slice name, AstType *type) {
    AstExprAnonFuncArg result = {
        .name = name,
        .type = type
    };
    return result;
}
