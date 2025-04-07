#include "anon_func.h"
#include "core/vec.h"
#include "sema/decl.h"
#include "sema/module.h"
#include "sema/nodes/shared/body.h"
#include "sema/nodes/shared/type.h"
#include "sema/scope.h"
#include "sema/type/type.h"
#include "sema/value.h"

SemaValue *sema_module_analyze_expr_anon_func(SemaModule *sema, FileLocation loc, AstExprAnonFunc *func, SemaExprCtx ctx) {
    SemaScopeStack ss = sema_scope_stack_new();
    SemaScopeStack *old_ss = sema_module_swap_ss(sema, &ss);
    SemaType *expectation = ctx.expectation && ctx.expectation->kind == SEMA_TYPE_FUNCTION ?
        ctx.expectation : NULL;
    sema_module_push_scope(sema, sema_scope_new(NULL));
    SemaType **args = vec_new(SemaType*);
    for (size_t i = 0; i < vec_len(func->args); i++) {
        AstExprAnonFuncArg *arg = &func->args[i];
        SemaType *type = NULL;
        if (!arg->type) {
            if (expectation && i < vec_len(expectation->func.args)) {
                type = expectation->func.args[i];
            } else {
                SEMA_ERROR(arg->loc, "failed to determine argument type");
            }
        } else {
            type = sema_resolve_type(sema, arg->type);
        }
        args = vec_push_dir(args, type);
        SemaDecl *decl = arg->sema.decl = sema_decl_new(
            arg->name,
            sema_value_new_runtime_var(type)
        );
        sema_module_push_decl(sema, arg->loc, decl, false);
    }
    SemaType *returns = NULL;
    if (!func->returns) {
        if (expectation) {
            returns = expectation->func.returns;
        } else {
            SEMA_ERROR(loc, "failed to determine returning type");
        }
    } else {
        returns = sema_resolve_type(sema, func->returns);
    }
    sema_module_push_scope(sema, sema_scope_new(returns));
    sema_analyze_body(sema, func->body);
    sema_module_swap_ss(sema, old_ss);
    return sema_value_new_runtime_final(sema_type_new_func(args, returns));
}
