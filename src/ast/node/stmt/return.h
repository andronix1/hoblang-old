#pragma once

#include <malloc.h>
#include "ast/interface/expr.h"
#include "sema/interface/body_break.h"

typedef struct {
    AstExpr *value;

    struct {
        SemaBodyBreak *body_break;
    } sema;
} AstReturn;

static inline AstReturn *ast_return_new(AstExpr *value) {
    AstReturn *result = malloc(sizeof(AstReturn));
    result->value = value;
    return result;
}

