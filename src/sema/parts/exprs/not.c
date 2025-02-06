#include "../../parts.h"

bool sema_analyze_expr_not(SemaModule *sema, AstExpr *not_expr, SemaType *expectation, SemaValue *value) { 
    SemaType *type = sema_value_expr_type(sema, not_expr, &primitives[PRIMITIVE_BOOL]);
    if (!sema_types_equals(type, &primitives[PRIMITIVE_BOOL])) {
        sema_err("not operator can be only be applied to booleans, not {sema::type}", type);
    }
    return sema_value_const(value, &primitives[PRIMITIVE_BOOL]);
}
