#include "exprs.h"
#include "sema/arch/bits/private.h"
#include "sema/type/private.h"

SemaValue *sema_analyze_expr_float(SemaModule *sema, long double integer, SemaExprCtx ctx) { 
	if (ctx.expectation && sema_type_is_float(ctx.expectation)) {
		return sema_value_const(ctx.expectation);
	} else {
        SemaType *ftype = sema_arch_float(sema);
        if (!ftype) {
            return NULL;
        }
		return sema_value_const(ftype);
	}
}
