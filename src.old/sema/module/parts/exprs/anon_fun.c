#include "ast/api/func_info.h"
#include "core/vec.h"
#include "exprs.h"
#include "sema/module/decls/api.h"
#include "sema/module/parts/body.h"
#include "sema/module/parts/func_info.h"
#include "sema/module/private.h"
#include "sema/module/scopes/api.h"
#include "sema/module/scopes/scope.h"
#include "sema/value/private.h"

SemaValue *sema_analyze_expr_anon_fun(SemaModule *sema, FileLocation at, AstExprAnonFun *anon_fun, SemaExprCtx ctx) {
    AstFuncTypeInfo *info = &anon_fun->info;
    SemaType *func_type = sema_ast_func(sema, at, NULL, info->args, info->returning);
    SemaScopeStack new_ss = sema_ss_new(info->returning->sema);
    SemaScopeStack *ss = sema_module_ss_swap(sema, &new_ss);
    if (!func_type) {
        sema_module_ss_swap(sema, ss);
        return NULL;
    }
    sema_module_push_scope(sema);
    for (size_t i = 0; i < vec_len(info->args); i++) {
        AstFuncArg *arg = &info->args[i];
        arg->decl = sema_module_push_scope_decl(sema, arg->loc, sema_decl_new(
            arg->name,
            sema_value_var(arg->type->sema)
        ));
    }
    sema_ast_body(sema, &anon_fun->body);
    sema_module_pop_scope(sema);
    sema_module_ss_swap(sema, ss);
    return sema_value_final(func_type);
}
