#include "ast/private/stmts/const.h"
#include "ast/private/expr.h"
#include "sema/type/api.h"
#include "sema/module/decls/api.h"
#include "sema/value/private.h"
#include "sema/module/private.h"
#include "sema/module/parts/expr.h"
#include "sema/module/parts/type.h"
#include "core/assert.h"

void sema_stmt_const(SemaModule *sema, FileLocation loc, bool is_global, bool public, AstConst *const_stmt) {
    assert(!(public && !is_global), "local constant cannot be public");
    SemaType *const_type = sema_ast_type(sema, &const_stmt->type);
    if (!const_type) {
        return;
    }
    SemaConst *constant = sema_const_expr(sema, const_stmt->expr, sema_expr_ctx_default_of(const_type));
    if (!constant) {
        return;
    }
    if (!sema_types_equals(const_type, constant->sema_type)) {
        SEMA_ERROR(const_stmt->expr->loc, "expected {sema::type} type of constant, not {sema::type}", const_type, constant->sema_type);
        return;
    }
    if (const_type) {
        if (is_global) {
            const_stmt->decl = sema_module_push_module_decl(sema, loc, public, sema_decl_new(
                const_stmt->name,
                sema_value_const(*constant)
            ));
        } else {
            const_stmt->decl = sema_module_push_scope_decl(sema, loc, sema_decl_new(
                const_stmt->name,
                sema_value_const(*constant)
            ));
        }
    }
}
