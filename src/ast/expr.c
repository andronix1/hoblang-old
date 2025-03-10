#include "ast/impl/expr.h"
#include "ast/private/expr.h"
#include "ast/private/body.h"
#include "ast/private/expr/as.h"
#include "ast/private/func_info.h"
#include "ast/private/type.h"
#include <malloc.h>

#define FIELD(src, dst) result->src = dst;
#define CONSTR(_type, fields) { \
        AstExpr *result = malloc(sizeof(AstExpr)); \
        result->value = NULL; \
        result->loc = loc; \
        result->scoped = false; \
        result->type = _type; \
        fields; \
        return result; \
    }

AstExpr *ast_expr_anon_fun(FileLocation loc, AstFuncTypeInfo info, AstBody body) CONSTR(AST_EXPR_ANON_FUN, {
    FIELD(anon_fun.info, info)
    FIELD(anon_fun.body, body)
})
AstExpr *ast_expr_struct(FileLocation loc, AstPath *path, AstExprStructMember *members) CONSTR(AST_EXPR_STRUCT, {
    FIELD(structure.path, path)
    FIELD(structure.members, members)
})
AstExpr *ast_expr_unwrap(FileLocation loc, AstExpr *expr, Slice name) CONSTR(AST_EXPR_UNWRAP, {
    FIELD(unwrap.expr, expr)
    FIELD(unwrap.name, name)
})
AstExpr *ast_expr_ret_on_null(FileLocation loc, AstExpr *expr) CONSTR(AST_EXPR_RET_ON_NULL, {
    FIELD(ret_on_null.expr, expr)
})
AstExpr *ast_expr_idx(FileLocation loc, AstExpr *of, AstExpr *idx) CONSTR(AST_EXPR_IDX, {
    FIELD(idx.of, of)
    FIELD(idx.idx, idx)
})
AstExpr *ast_expr_get_local_path(FileLocation loc, AstPath *path) CONSTR(AST_EXPR_GET_LOCAL_PATH, {
    FIELD(get_local.path, path)
})
AstExpr *ast_expr_get_inner_path(FileLocation loc, AstExpr *of, AstPath *path) CONSTR(AST_EXPR_GET_INNER_PATH, {
    FIELD(get_inner.of, of)
    FIELD(get_inner.path, path)
})
AstExpr *ast_expr_integer(FileLocation loc, uint64_t value) CONSTR(AST_EXPR_INTEGER, {
    FIELD(integer, value)
})
AstExpr *ast_expr_float(FileLocation loc, long double value) CONSTR(AST_EXPR_FLOAT, {
    FIELD(float_value, value)
})
AstExpr *ast_expr_char(FileLocation loc, char value) CONSTR(AST_EXPR_CHAR, {
    FIELD(character, value)
})
AstExpr *ast_expr_str(FileLocation loc, Slice value) CONSTR(AST_EXPR_STR, {
    FIELD(str, value)
})
AstExpr *ast_expr_bool(FileLocation loc, bool value) CONSTR(AST_EXPR_BOOL, {
    FIELD(boolean, value)
})
AstExpr *ast_expr_call(FileLocation loc, AstExpr *callable, AstExpr **args) CONSTR(AST_EXPR_CALL, {
    FIELD(call.callable, callable)
    FIELD(call.args, args)
})
AstExpr *ast_expr_not(FileLocation loc, AstExpr *expr) CONSTR(AST_EXPR_NOT, {
    FIELD(not_expr, expr)
})
#include "core/print.h"
AstExpr *ast_expr_as_type(FileLocation loc, FileLocation as_loc, AstExpr *expr, AstType *type) CONSTR(AST_EXPR_AS, {
    FIELD(as.loc, as_loc)
    FIELD(as.type, AST_EXPR_AS_TYPE)
    FIELD(as.expr, expr)
    FIELD(as.as_type, type)
})
AstExpr *ast_expr_as_auto(FileLocation loc, FileLocation as_loc, AstExpr *expr) CONSTR(AST_EXPR_AS, {
    FIELD(as.loc, as_loc)
    FIELD(as.type, AST_EXPR_AS_AUTO)
    FIELD(as.expr, expr)
})
AstExpr *ast_expr_binop(FileLocation loc, AstBinopType type, AstExpr *left, AstExpr *right) CONSTR(AST_EXPR_BINOP, {
    FIELD(binop.type, type)
    FIELD(binop.left, left)
    FIELD(binop.right, right)
})
AstExpr *ast_expr_unary(FileLocation loc, AstUnaryType type, AstExpr *expr) CONSTR(AST_EXPR_UNARY, {
    FIELD(unary.type, type)
    FIELD(unary.expr, expr)
})
AstExpr *ast_expr_array(FileLocation loc, AstExpr **values) CONSTR(AST_EXPR_ARRAY, {
    FIELD(array, values)
})
AstExpr *ast_expr_null(FileLocation loc) CONSTR(AST_EXPR_NULL, {})
AstExpr *ast_expr_ref(FileLocation loc, AstExpr *expr) CONSTR(AST_EXPR_REF, {
    FIELD(ref_expr, expr)
})
