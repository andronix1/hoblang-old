#include "../../parts.h"

SemaType *sema_ast_expr_array(SemaModule *sema, AstExpr **array, SemaType *expectation) {
	SemaType *expect = NULL;
	if (expectation && expectation->type == SEMA_TYPE_ARRAY) {
		expect = expectation->array.of;
	}
	if (vec_len(array) > 0) {
		if (!(expect = sema_ast_expr_type(sema, array[0], expect))) {
			return NULL;
		}
	} else if (!expectation) {
		sema_err("cannot infer array type");
		return NULL;
	} else {
		if (expectation->type != SEMA_TYPE_POINTER) {
			sema_err("expected ptr but got {sema::type}", expectation);
			return NULL;
		}
		expect = expectation->ptr_to;
	}
	for (size_t i = 1; i < vec_len(array); i++) {
		SemaType *stype = sema_ast_expr_type(sema, array[i], expect);	
		if (!stype) {
			return NULL;
		}
		if (!sema_types_equals(expect, stype)) {
			sema_err("different types in one array: {sema::type} and {sema::type}", expect, &stype);
			return NULL;
		}
	}
	return sema_type_new_array(vec_len(array), expect);
}
