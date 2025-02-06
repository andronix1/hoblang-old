#include "../../parts.h"

bool sema_analyze_expr_int(SemaModule *sema, uint64_t integer, SemaType *expectation, SemaValue *value) { 
	if (expectation && expectation->type == SEMA_TYPE_PRIMITIVE && expectation->primitive != PRIMITIVE_VOID) {
		return sema_value_const(value, expectation);
	} else {
        // TODO: i32
		return sema_value_const(value, &primitives[PRIMITIVE_I32]);
	}
}
