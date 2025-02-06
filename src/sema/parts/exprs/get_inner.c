#include "../../parts.h"

bool sema_analyze_expr_get_inner(SemaModule *sema, AstExprGetInner *get_inner, SemaType *expectation, SemaValue *value) { 
    SemaType *of_type = sema_value_expr_type(sema, get_inner->of, NULL);
    if (!of_type) {
        return false;
    }
    return sema_resolve_inner_value_path(sema, of_type, &get_inner->path, 0, value);
}
