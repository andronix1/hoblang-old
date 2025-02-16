#include "exprs.h"
#include "sema/type/private.h"

SemaValue *sema_analyze_expr_float(SemaModule *sema, long double integer, SemaExprCtx ctx) { 
	if (ctx.expectation && sema_type_is_float(ctx.expectation)) {
		return sema_value_const(ctx.expectation);
	} else {
		return sema_value_const(sema_type_primitive_f32());
	}
}
