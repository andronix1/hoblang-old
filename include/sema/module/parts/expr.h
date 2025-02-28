#pragma once

#include <stddef.h>
#include "ast/api/expr.h"
#include "core/location.h"
#include "sema/type/type.h"
#include "sema/module.h"
#include "sema/value.h"
#include "sema/const/const.h"

typedef struct {
	SemaType *expectation;
    FileLocation loc;
} SemaExprCtx;

SemaValue *sema_callable_expr_type(SemaModule *sema, AstExpr *expr, SemaExprCtx ctx);
SemaType *sema_value_expr_type(SemaModule *sema, AstExpr *expr, SemaExprCtx ctx);
SemaType *sema_var_expr_type(SemaModule *sema, AstExpr *expr, SemaExprCtx ctx);
SemaConst *sema_const_expr(SemaModule *sema, AstExpr *expr, SemaExprCtx ctx);

inline static SemaExprCtx sema_expr_ctx_default() {
	SemaExprCtx ctx = {
		.expectation = NULL
	};
	return ctx;
}

inline static SemaExprCtx sema_expr_ctx_expect(SemaExprCtx ctx, SemaType *type) {
	ctx.expectation = type;
    ctx.loc = ctx.loc;
	return ctx;
}

inline static SemaExprCtx sema_expr_ctx_default_of(SemaType *type) {
	return sema_expr_ctx_expect(
		sema_expr_ctx_default(),
		type
	);
}
