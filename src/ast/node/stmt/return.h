#pragma once

#include <malloc.h>
#include "ast/interface/expr.h"

typedef struct {
    AstExpr *value;
} AstReturn;

static inline AstReturn *ast_return_new(AstExpr *value) {
    AstReturn *result = malloc(sizeof(AstReturn));
    result->value = value;
    return result;
}

