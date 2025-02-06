#include "../../parts.h"

bool sema_analyze_expr_unary(SemaModule *sema, AstExprUnary *unary, SemaType *expectation, SemaValue *value) { 
    SemaType *type = sema_value_expr_type(sema, unary->expr, expectation);
    if (!type) {
        return NULL;
    }
    switch (unary->type) {
        case AST_UNARY_MINUS:
            if (type->type != SEMA_TYPE_PRIMITIVE) {
                sema_err("cannot apply unary minus to non-primitive types");
            }
            return type;
        case AST_UNARY_BITNOT:
            if (type->type != SEMA_TYPE_PRIMITIVE) {
                sema_err("cannot apply unary bitnot to non-primitive types");
            }
            return type;
    }
    assert(0, "invalid unary type: {int}", unary->type);
}
