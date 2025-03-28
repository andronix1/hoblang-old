#include "exprs.h"
#include "sema/arch/bits/private.h"
#include "sema/type/private.h"
#include "sema/const/api.h"

SemaValue *sema_analyze_expr_int(SemaModule *sema, uint64_t integer, SemaExprCtx ctx) { 
    SemaType *type = NULL;
	if (ctx.expectation && sema_type_is_int(ctx.expectation)) {
        type = ctx.expectation;
	} else {
        type = sema_arch_int(sema);
	}
	return sema_value_const(sema_const_int(type, integer));
}
