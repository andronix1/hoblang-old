#include "exprs.h"
#include "sema/arch/bits/private.h"
#include "sema/type/private.h"
#include "sema/const/api.h"
#include "sema/value/private.h"

SemaValue *sema_analyze_expr_float(SemaModule *sema, long double value, SemaExprCtx ctx) { 
    SemaType *type = NULL;
	if (ctx.expectation && sema_type_is_float(ctx.expectation)) {
        type = ctx.expectation;
	} else {
        type = sema_arch_float(sema);
        if (!type) {
            return NULL;
        }
	}
    return sema_value_const(sema_const_float(type, value));
}
