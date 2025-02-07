#pragma once

#include "../type.h"

typedef struct {
	SemaType *expectation;
} SemaExprCtx;

inline static SemaExprCtx sema_expr_ctx_default() {
	SemaExprCtx ctx = {
		.expectation = NULL
	};
	return ctx;
}

inline static SemaExprCtx sema_expr_ctx_expect(SemaExprCtx ctx, SemaType *type) {
	ctx.expectation = type;
	return ctx;
}

inline static SemaExprCtx sema_expr_ctx_default_of(SemaType *type) {
	return sema_expr_ctx_expect(
		sema_expr_ctx_default(),
		type
	);
}
