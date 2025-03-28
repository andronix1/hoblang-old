#pragma once

#include "ast/interface/expr.h"
#include "sema/interface/module.h"
#include "sema/interface/type.h"
#include "sema/interface/value.h"

typedef struct {
    SemaType *expectation;
} SemaExprCtx;

static inline SemaExprCtx sema_expr_ctx_new(SemaType *expectation) {
    SemaExprCtx result = {
        .expectation = expectation
    };
    return result;
}

static inline SemaExprCtx sema_expr_ctx_with(SemaExprCtx ctx __attribute__((unused)), SemaType *expectation) {
    return sema_expr_ctx_new(expectation);
}

SemaValue *sema_analyze_expr(SemaModule *sema, AstExpr *expr, SemaExprCtx ctx);
SemaType *sema_analyze_runtime_expr(SemaModule *sema, AstExpr *expr, SemaExprCtx ctx);
