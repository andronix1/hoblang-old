#include "binop.h"
#include "ast/interface/expr.h"
#include "ast/shared/expr.h"
#include "core/not_null.h"
#include "sema/interface/type.h"
#include "sema/interface/value.h"
#include "sema/nodes/shared/expr.h"
#include "sema/module.h"
#include "sema/type/type.h"
#include "sema/value.h"
#include "core/assert.h"

typedef enum {
	SEMA_BINOP_INT_ARITHM,
	SEMA_BINOP_ARITHM,
	SEMA_BINOP_COMP_EQ,
	SEMA_BINOP_COMP_ORDER,
	SEMA_BINOP_BOOL,
	SEMA_BINOP_BINARY,
} SemaBinopKind;

#define SEMA_FINAL sema_value_new_runtime(sema_value_runtime_new_final(type))
#define SEMA_FINAL_BOOL sema_value_new_runtime(sema_value_runtime_new_final(sema_type_primitive_bool()))

SemaBinopKind sema_binop_kind(AstBinopKind kind) {
    switch (kind) {
        case AST_BINOP_MOD:
            return SEMA_BINOP_INT_ARITHM;
        case AST_BINOP_ADD:
        case AST_BINOP_SUB:
        case AST_BINOP_MUL:
        case AST_BINOP_DIV:
            return SEMA_BINOP_ARITHM;
        case AST_BINOP_EQ:
        case AST_BINOP_NEQ:
            return SEMA_BINOP_COMP_EQ;
        case AST_BINOP_GT:
        case AST_BINOP_GE:
        case AST_BINOP_LT:
        case AST_BINOP_LE:
            return SEMA_BINOP_COMP_ORDER;
        case AST_BINOP_OR:   
        case AST_BINOP_AND:
            return SEMA_BINOP_BOOL;
        case AST_BINOP_BITAND:
        case AST_BINOP_BITOR:
        case AST_BINOP_SHR:
        case AST_BINOP_SHL:
        case AST_BINOP_XOR:
            return SEMA_BINOP_BINARY;
    }
    assert(0, "illegal ast binop kind");
}

SemaValue *sema_module_analyze_expr_binop(SemaModule *sema, AstExprBinop *binop) {
    SemaType *left = sema_value_is_runtime(NOT_NULL(sema_analyze_expr(sema, binop->left, sema_expr_ctx_new(NULL))));
    if (!left) {
        SEMA_ERROR(binop->left->loc, "left expression must be a runtime value");
        return NULL;
    }
    SemaType *right = sema_value_is_runtime(NOT_NULL(sema_analyze_expr(sema, binop->right, sema_expr_ctx_new(left))));
    if (!right) {
        SEMA_ERROR(binop->right->loc, "right expression must be a runtime value");
        return NULL;
    }
    if (!sema_types_equals(left, right)) {
        SEMA_ERROR(binop->binop_loc, "binop can be applied to identical typed only, but {sema::type} != {sema::type}", left, right);
        return NULL;
    }
    SemaType *type = left;
    switch (sema_binop_kind(binop->kind)) {
        case SEMA_BINOP_INT_ARITHM:
            if (!sema_type_is_int(type)) {
                SEMA_ERROR(binop->binop_loc, "`{ast::binop}` can be applied to integers only, not {sema::type}", type);
                return NULL;
            }
            return SEMA_FINAL;
        case SEMA_BINOP_BINARY:
        case SEMA_BINOP_ARITHM:
            if (!sema_type_is_int(type) && !sema_type_is_float(type)) {
                SEMA_ERROR(binop->binop_loc, "`{ast::binop}` can be applied to integers or float only, not {sema::type}", type);
                return NULL;
            }
            return SEMA_FINAL;
        case SEMA_BINOP_COMP_EQ:
            return SEMA_FINAL_BOOL;
        case SEMA_BINOP_COMP_ORDER:
            if (!sema_type_is_int(type) && !sema_type_is_float(type)) {
                SEMA_ERROR(binop->binop_loc, "only integers and floats can be compared, not {sema::type}", type);
                return NULL;
            }
            return SEMA_FINAL_BOOL;
        case SEMA_BINOP_BOOL:
            if (!sema_types_equals(type, sema_type_primitive_bool())) {
                SEMA_ERROR(binop->binop_loc, "`{ast::binop}` can be applied to booleans only, not {sema::type}", type);
                return NULL;
            }
            return SEMA_FINAL;
    }
    assert(0, "invalid sema binop kind");
}
