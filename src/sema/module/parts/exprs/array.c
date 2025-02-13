#include "core/vec.h"
#include "sema/type/private.h"
#include "exprs.h"

SemaValue *sema_analyze_expr_array(SemaModule *sema, AstExpr **array, SemaExprCtx ctx) {
	SemaType *expect = false;
	if (ctx.expectation && ctx.expectation->type == SEMA_TYPE_ARRAY) {
		expect = ctx.expectation->array.of;
	}
	if (vec_len(array) > 0) {
		if (!(expect = sema_value_expr_type(sema, array[0], sema_expr_ctx_expect(ctx, expect)))) {
			return NULL;
		}
	} else if (!ctx.expectation) {
		sema_err("cannot infer array type");
		return NULL;
	} else {
		if (ctx.expectation->type != SEMA_TYPE_POINTER) {
			sema_err("expected ptr but got {sema::type}", ctx.expectation);
			return NULL;
		}
		expect = ctx.expectation->ptr_to;
	}
	for (size_t i = 1; i < vec_len(array); i++) {
		SemaType *stype = sema_value_expr_type(sema, array[i], sema_expr_ctx_expect(ctx, expect));
		if (!stype) {
			return NULL;
		}
		if (!sema_types_equals(expect, stype)) {
			sema_err("different types in one array: {sema::type} and {sema::type}", expect, &stype);
			return NULL;
		}
	}
	return sema_value_const(sema_type_new_array(vec_len(array), expect));
}
