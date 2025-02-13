#include "exprs.h"
#include "sema/type/private.h"

SemaValue *sema_analyze_expr_int(SemaModule *sema, uint64_t integer, SemaType *expectation) { 
	if (expectation && expectation->type == SEMA_TYPE_PRIMITIVE && expectation->primitive != PRIMITIVE_VOID) {
		return sema_value_const(expectation);
	} else {
        // TODO: i32
		return sema_value_const(sema_type_primitive_i32());
	}
}
