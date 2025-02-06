#include "../../parts.h"

bool sema_analyze_expr_array(SemaModule *sema, AstExpr **array, SemaType *expectation, SemaValue *value) {
	SemaType *expect = false;
	if (expectation && expectation->type == SEMA_TYPE_ARRAY) {
		expect = expectation->array.of;
	}
	if (vec_len(array) > 0) {
		if (!(expect = sema_value_expr_type(sema, array[0], expect))) {
			return false;
		}
	} else if (!expectation) {
		sema_err("cannot infer array type");
		return false;
	} else {
		if (expectation->type != SEMA_TYPE_POINTER) {
			sema_err("expected ptr but got {sema::type}", expectation);
			return false;
		}
		expect = expectation->ptr_to;
	}
	for (size_t i = 1; i < vec_len(array); i++) {
		SemaType *stype = sema_value_expr_type(sema, array[i], expect);	
		if (!stype) {
			return false;
		}
		if (!sema_types_equals(expect, stype)) {
			sema_err("different types in one array: {sema::type} and {sema::type}", expect, &stype);
			return false;
		}
	}
	return sema_value_const(value, sema_type_new_array(vec_len(array), expect));
}
