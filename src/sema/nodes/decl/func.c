#include "func.h"
#include "ast/shared/func_info.h"
#include "core/not_null.h"
#include "core/slice/api.h"
#include "core/vec.h"
#include "sema/decl.h"
#include "sema/module.h"
#include "sema/nodes/shared/body.h"
#include "sema/nodes/shared/func_info.h"
#include "sema/nodes/shared/type.h"
#include "sema/value.h"

void sema_module_read_func(SemaModule *sema, AstFuncDecl *decl, bool is_generic, bool public) {
    if (is_generic) {
        sema_resolve_func_info_type(sema, decl->info);
    } else {
        sema_read_func_info(sema, decl->info, public); 
    }
}

void sema_analyze_func_decl(SemaModule *sema, AstFuncDecl *func) {
    SemaType *returns = RET_ON_NULL(sema_resolve_type(sema, func->info->returns));
    SemaScopeStack new_ss = sema_scope_stack_new();
    SemaScopeStack *ss = sema_module_swap_ss(sema, &new_ss);
    sema_module_push_scope(sema, sema_scope_new(returns));
    if (func->info->ext_of) {
        SemaType *type = sema_resolve_type(sema, func->info->ext_of);
        if (type) {
            SemaDecl *self = func->sema.self = sema_decl_new(
                slice_from_cstr("self"),
                sema_value_new_runtime(sema_value_runtime_new_final(type))
            );
            sema_module_push_decl(sema, func->info->name_loc, self, false);
        }
    }
    for (size_t i = 0; i < vec_len(func->info->args); i++) {
        AstFuncInfoArg *arg = &func->info->args[i];
        SemaType *type = sema_resolve_type(sema, arg->type);
        if (!type) {
            continue;
        }
        SemaDecl *decl = arg->sema.decl = sema_decl_new(arg->name,
            sema_value_new_runtime(sema_value_runtime_new_final(type)));
        sema_module_push_decl(sema, arg->name_loc, decl, false);
    }
    sema_analyze_body(sema, func->body);
    sema_module_swap_ss(sema, ss);
}
