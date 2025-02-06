#include "../parts.h"

bool sema_analyze_expr_array(SemaModule *sema, AstExpr **array, SemaType *expectation, SemaValue *value);
bool sema_analyze_expr_as(SemaModule *sema, AstExprAs *as, SemaType *expectation, SemaValue *value);
bool sema_analyze_expr_binop(SemaModule *sema, AstExprBinop *binop, SemaType *expectation, SemaValue *value);
bool sema_analyze_expr_call(SemaModule *sema, AstCall *call, SemaType *expectation, SemaValue *value);
bool sema_analyze_expr_int(SemaModule *sema, uint64_t integer, SemaType *expectation, SemaValue *value);
bool sema_analyze_expr_ref(SemaModule *sema, AstExpr *ref_expr, SemaType *expectation, SemaValue *value);
bool sema_analyze_expr_not(SemaModule *sema, AstExpr *not_expr, SemaType *expectation, SemaValue *value);
bool sema_analyze_expr_unary(SemaModule *sema, AstExprUnary *unary, SemaType *expectation, SemaValue *value);
bool sema_analyze_expr_get_inner(SemaModule *sema, AstExprGetInner *get_inner, SemaType *expectation, SemaValue *value);
bool sema_analyze_expr_get_local(SemaModule *sema, AstExprGetLocal *get_local, SemaType *expectation, SemaValue *value);

bool sema_analyze_expr(SemaModule *sema, AstExpr *expr, SemaType *expectation, SemaValue *value) {
    switch (expr->type) {
		case AST_EXPR_UNARY: if (!sema_analyze_expr_unary(sema, &expr->unary, expectation, value)) { return false; } break;
		case AST_EXPR_GET_INNER_PATH: if (!sema_analyze_expr_get_inner(sema, &expr->get_inner, expectation, value)) { return false; } break;
		case AST_EXPR_GET_LOCAL_PATH: if (!sema_analyze_expr_get_local(sema, &expr->get_local, expectation, value)) { return false; } break;
		case AST_EXPR_REF: if (!sema_analyze_expr_ref(sema, expr->ref_expr, expectation, value)) { return false; } break;
		case AST_EXPR_NOT: if (!sema_analyze_expr_not(sema, expr->not_expr, expectation, value)) { return false; } break;
		case AST_EXPR_STR: if (!sema_value_const(value, sema_type_new_slice(&primitives[PRIMITIVE_U8]))) { return false; } break;
		case AST_EXPR_CHAR: if (!sema_value_const(value, &primitives[PRIMITIVE_U8])) { return false; } break;
		case AST_EXPR_BOOL: if (!sema_value_const(value, &primitives[PRIMITIVE_BOOL])) { return false; } break;
		case AST_EXPR_ARRAY: if (!sema_analyze_expr_array(sema, expr->array, expectation, value)) { return false; } break;
		case AST_EXPR_BINOP: if (!sema_analyze_expr_binop(sema, &expr->binop, expectation, value)) { return false; } break;
		case AST_EXPR_AS: if (!sema_analyze_expr_as(sema, &expr->as, expectation, value)) { return false; } break;
		case AST_EXPR_INTEGER: if (!sema_analyze_expr_int(sema, expr->integer, expectation, value)) { return false; } break;
        case AST_EXPR_CALL: if (!sema_analyze_expr_call(sema, &expr->call, expectation, value)) { return false; } break;
    }
    expr->sema_type = value->sema_type;
    return true;
}

SemaType *sema_var_expr_type(SemaModule *sema, AstExpr *expr, SemaType *expectation) {
    SemaValue value;
    if (!sema_analyze_expr(sema, expr, expectation, &value)) {
        return NULL;
    }
    if (value.type != SEMA_VALUE_VAR) {
        sema_err("`{ast::expr}` is not a variable", expr);
    }
    return value.sema_type;
}
SemaType *sema_value_expr_type(SemaModule *sema, AstExpr *expr, SemaType *expectation) {
    SemaValue value;
    if (!sema_analyze_expr(sema, expr, expectation, &value)) {
        return NULL;
    }
    if (value.type != SEMA_VALUE_CONST && value.type != SEMA_VALUE_VAR) {
        sema_err("`{ast::expr}` is not a value", expr);
    }
    return value.sema_type;
}
