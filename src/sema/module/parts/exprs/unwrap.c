#include "ast/private/expr.h"
#include "exprs.h"
#include "sema/module/private.h"
#include "sema/module/decls/impl.h"
#include "sema/type/private.h"
#include "sema/value/private.h"

SemaValue *sema_analyze_expr_unwrap(SemaModule *sema, AstExprUnwrap *unwrap, SemaExprCtx ctx) {
    SemaType *expr_type = sema_value_expr_type(sema, unwrap->expr, sema_expr_ctx_expect(ctx, NULL));
    if (!expr_type) {
        return NULL;
    }
    if (expr_type->type != SEMA_TYPE_OPTIONAL) {
        sema_err("expected expr type to be optional");
        return NULL;
    }
    unwrap->type = SEMA_EXPR_UNWRAP_OPT;
    unwrap->decl = &sema_module_push_decl(sema, sema_scope_decl_new_value(
        unwrap->name,
        expr_type->optional_of,
        true
    ))->value_decl;
    return sema_value_const(sema_type_primitive_bool());
}
