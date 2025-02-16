#include "ast/private/expr.h"
#include <malloc.h>

#define FIELD(src, dst) result->src = dst;
#define CONSTR(_type, fields) { \
        AstExpr *result = malloc(sizeof(AstExpr)); \
        result->scoped = false; \
        result->type = _type; \
        fields; \
        return result; \
    }

AstExpr *ast_expr_idx(AstExpr *of, AstExpr *idx) CONSTR(AST_EXPR_IDX, {
    FIELD(idx.of, of)
    FIELD(idx.idx, idx)
})
AstExpr *ast_expr_get_local_path(AstPath path) CONSTR(AST_EXPR_GET_LOCAL_PATH, {
    FIELD(get_local.path, path)
})
AstExpr *ast_expr_get_inner_path(AstExpr *of, AstInnerPath path) CONSTR(AST_EXPR_GET_INNER_PATH, {
    FIELD(get_inner.of, of)
    FIELD(get_inner.path, path)
})
AstExpr *ast_expr_integer(uint64_t value) CONSTR(AST_EXPR_INTEGER, {
    FIELD(integer, value)
})
AstExpr *ast_expr_float(long double value) CONSTR(AST_EXPR_FLOAT, {
    FIELD(float_value, value)
})
AstExpr *ast_expr_char(char value) CONSTR(AST_EXPR_CHAR, {
    FIELD(character, value)
})
AstExpr *ast_expr_str(Slice value) CONSTR(AST_EXPR_STR, {
    FIELD(str, value)
})
AstExpr *ast_expr_bool(bool value) CONSTR(AST_EXPR_BOOL, {
    FIELD(boolean, value)
})
AstExpr *ast_expr_call(AstExpr *callable, AstExpr **args) CONSTR(AST_EXPR_CALL, {
    FIELD(call.callable, callable)
    FIELD(call.args, args)
})
AstExpr *ast_expr_not(AstExpr *expr) CONSTR(AST_EXPR_NOT, {
    FIELD(not_expr, expr)
})
AstExpr *ast_expr_as(AstExpr *expr, AstType type) CONSTR(AST_EXPR_AS, {
    FIELD(as.expr, expr)
    FIELD(as.type, type)
})
AstExpr *ast_expr_binop(AstBinopType type, AstExpr *left, AstExpr *right) CONSTR(AST_EXPR_BINOP, {
    FIELD(binop.type, type)
    FIELD(binop.left, left)
    FIELD(binop.right, right)
})
AstExpr *ast_expr_unary(AstUnaryType type, AstExpr *expr) CONSTR(AST_EXPR_UNARY, {
    FIELD(unary.type, type)
    FIELD(unary.expr, expr)
})
AstExpr *ast_expr_array(AstExpr **values) CONSTR(AST_EXPR_ARRAY, {
    FIELD(array, values)
})
AstExpr *ast_expr_ref(AstExpr *expr) CONSTR(AST_EXPR_REF, {
    FIELD(ref_expr, expr)
})
