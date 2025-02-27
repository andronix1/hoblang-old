#include "exprs.h"
#include "sema/arch/bits/private.h"
#include "sema/type/private.h"

SemaValue *sema_analyze_expr_int(SemaModule *sema, uint64_t integer, SemaExprCtx ctx) { 
	if (ctx.expectation && sema_type_is_int(ctx.expectation)) {
		return sema_value_const(ctx.expectation);
	} else {
		return sema_value_const(sema_arch_int(sema));
	}
}
