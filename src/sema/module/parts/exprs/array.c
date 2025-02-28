#include "core/vec.h"
#include "sema/const/api.h"
#include "sema/module/private.h"
#include "sema/type/private.h"
#include "exprs.h"
#include "sema/value/private.h"

SemaValue *sema_analyze_expr_array(SemaModule *sema, AstExpr **array, SemaExprCtx ctx) {
	SemaType *expect = false;
	if (ctx.expectation && ctx.expectation->type == SEMA_TYPE_ARRAY) {
		expect = ctx.expectation->array.of;
	}
	if (vec_len(array) > 0) {
		if (!(expect = sema_value_expr_type(sema, array[0], sema_expr_ctx_expect(ctx, expect)))) {
			return NULL;
		}
	} else if (!ctx.expectation) {
		SEMA_ERROR(ctx.loc, "cannot infer array type");
		return NULL;
	} else {
		if (ctx.expectation->type != SEMA_TYPE_ARRAY) {
			SEMA_ERROR(ctx.loc, "expected array but got {sema::type}", ctx.expectation);
			return NULL;
		}
		expect = ctx.expectation->array.of;
	}
    bool is_const = true;
	for (size_t i = 1; i < vec_len(array); i++) {
		SemaType *stype = sema_value_expr_type(sema, array[i], sema_expr_ctx_expect(ctx, expect));
		if (!stype) {
			return NULL;
		}
        if (array[i]->value->type != SEMA_VALUE_CONST) {
            is_const = false;
        }
		if (!sema_types_equals(expect, stype)) {
			SEMA_ERROR(array[i]->loc, "element of type {sema::type} in array initializer of {sema::type}", &stype, expect);
			return NULL;
		}
	}
    if (is_const) {
        SemaConst *elements = vec_new(SemaConst);
        for (size_t i = 0; i < vec_len(array); i++) {
            elements = vec_push(elements, &array[i]->value->constant);
        }
        return sema_value_const(sema_const_array(expect, elements));
    } else {
	    return sema_value_final(sema_type_new_array(vec_len(array), expect));
    }
}
