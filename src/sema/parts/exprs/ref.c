#include "../../parts.h"

bool sema_analyze_expr_ref(SemaModule *sema, AstExpr *ref_expr, SemaType *expectation, SemaValue *value) { 
    SemaType *type = sema_var_expr_type(sema, ref_expr, NULL);
    if (!type) {
        return false;
    }
    return sema_value_var(value, sema_type_new_pointer(type));
}

