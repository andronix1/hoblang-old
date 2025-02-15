#include "sema/module/decls/impl.h"
#include "eval.h"
#include "ast/private/expr.h"
#include "core/vec.h"
#include "core/assert.h"
#include "sema/type/private.h"
#include "sema/module/private.h"
#include "sema/module/parts/path.h"
#include "sema/value/private.h"

uint64_t sema_eval_int_expr(SemaModule *sema, AstExpr *expr) {
    switch (expr->type) {
        case AST_EXPR_GET_INNER_PATH:
        case AST_EXPR_STR:
        case AST_EXPR_BOOL:
        case AST_EXPR_CALL:
        case AST_EXPR_NOT:
        case AST_EXPR_AS:
        case AST_EXPR_BINOP:
        case AST_EXPR_UNARY:
        case AST_EXPR_ARRAY:
        case AST_EXPR_IDX:
        case AST_EXPR_FLOAT:
        case AST_EXPR_REF:
            sema_err("cannot use {ast::expr} in constant int expr", expr);
            return 0;
        case AST_EXPR_INTEGER:
            return expr->integer;
        case AST_EXPR_CHAR:
            return expr->character;
        case AST_EXPR_GET_LOCAL_PATH: {
            if (vec_len(expr->get_local.path.inner_path.segments)) {
                sema_err("only decl path constant expr are supported");
                return 0;
            }
            SemaValue *decl = sema_resolve_decl_path(sema, &expr->get_local.path.decl_path);
            if (!decl) {
                return 0;
            }
            if (decl->type != SEMA_VALUE_CONST || decl->sema_type->type != SEMA_TYPE_PRIMITIVE) {
                sema_err("decl is not a constant integer");
                return 0;
            }
            return sema_eval_int_expr(sema, decl->integer_expr);
        }
    }
    assert(0, "invalid expr type {int}", expr->type);
}
