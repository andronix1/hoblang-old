#pragma once

#include "ast/interface/expr.h"
#include "ast/interface/body.h"
#include "core/slice/slice.h"
#include "sema/interface/loop.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    AstExpr *cond;
    AstBody *body;
    bool is_named;
    Slice name;

    struct {
        SemaLoop *loop;
    } sema;
} AstWhile;

static inline AstWhile *ast_while_new_named(AstExpr *cond, AstBody *body, Slice name) {
    AstWhile *result = malloc(sizeof(AstWhile));
    result->cond = cond;
    result->body = body;
    result->is_named = true;
    result->name = name;
    return result;
}

static inline AstWhile *ast_while_new(AstExpr *cond, AstBody *body) {
    AstWhile *result = malloc(sizeof(AstWhile));
    result->cond = cond;
    result->body = body;
    result->is_named = false;
    return result;
}
