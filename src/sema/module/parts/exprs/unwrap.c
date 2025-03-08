#include "ast/private/expr.h"
#include "exprs.h"
#include "sema/module/private.h"
#include "sema/module/decls/impl.h"
#include "sema/type/private.h"
#include "sema/value/private.h"
#include "sema/module/decls/api.h"

SemaValue *sema_analyze_expr_unwrap(SemaModule *sema, AstExprUnwrap *unwrap, SemaExprCtx ctx) {
    SemaType *expr_type = sema_value_expr_type(sema, unwrap->expr, sema_expr_ctx_expect(ctx, NULL));
    if (!expr_type) {
        return NULL;
    }
    if (expr_type->type != SEMA_TYPE_OPTIONAL) {
        SEMA_ERROR(ctx.loc, "only optionals can be unwrapped, not {sema::type}", expr_type);
        return NULL;
    }
    unwrap->type = SEMA_EXPR_UNWRAP_OPT;
    unwrap->decl = sema_module_push_scope_decl(sema, ctx.loc, sema_decl_new(
        unwrap->name,
        sema_value_final(expr_type->optional_of)
    ));
    return sema_value_final(sema_type_primitive_bool());
}
