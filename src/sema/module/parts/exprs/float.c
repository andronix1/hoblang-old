#include "exprs.h"
#include "sema/type/private.h"

SemaValue *sema_analyze_expr_float(SemaModule *sema, uint64_t integer, SemaType *expectation) { 
	if (expectation && sema_type_is_float(expectation)) {
		return sema_value_const(expectation);
	} else {
		return sema_value_const(sema_type_primitive_f32());
	}
}
