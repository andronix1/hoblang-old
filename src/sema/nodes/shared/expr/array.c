#include "array.h"
#include "ast/shared/expr.h"
#include "core/location.h"
#include "core/vec.h"
#include "sema/interface/type.h"
#include "sema/interface/value.h"
#include "sema/module.h"
#include "sema/type/type.h"
#include "sema/value.h"

SemaValue *sema_module_analyze_expr_array(SemaModule *sema, FileLocation loc, AstExprArray *array, SemaExprCtx ctx) {
    SemaType *expected = ctx.expectation && ctx.expectation->kind == SEMA_TYPE_ARRAY ?
        ctx.expectation->array.of : NULL;
    for (size_t i = 0; i < vec_len(array->values); i++) {
        SemaValue *value = sema_analyze_expr(sema, array->values[i], sema_expr_ctx_with(ctx, expected));
        if (!value) {
            continue;
        }
        SemaType *type = sema_value_is_runtime(value);
        if (!type) { 
            SEMA_ERROR(array->values[i]->loc, "array elements must be runtime values");
            continue;
        }
        if (expected && !sema_types_equals(expected, type)) {
            SEMA_ERROR(array->values[i]->loc, "expected array element to be {sema::type} but got element of type {sema::type}", expected, type);
            continue; 
        }
        if (!expected) {
            expected = type;
        }
    }
    if (!expected) {
        SEMA_ERROR(loc, "failed to determine array type");
        return NULL;
    }
    return sema_value_new_runtime(sema_value_runtime_new_final(sema_type_new_array(vec_len(array->values), expected)));
}
