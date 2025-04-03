#include "func_info.h"
#include "ast/shared/func_info.h"
#include "core/not_null.h"
#include "core/vec.h"
#include "sema/decl.h"
#include "sema/module.h"
#include "sema/type/type.h"
#include "sema/value.h"
#include "sema/nodes/shared/type.h"

SemaType *sema_resolve_func_info_type(SemaModule *sema, AstFuncInfo *func_info) {
    SemaType *returns = NOT_NULL(sema_resolve_type(sema, func_info->returns));
    SemaType **args = vec_new(SemaType*);
    if (func_info->ext_of) {
        args = vec_push_dir(args, NOT_NULL(sema_resolve_type(sema, func_info->ext_of))); 
    }
    for (size_t i = 0; i < vec_len(func_info->args); i++) {
        args = vec_push_dir(args, NOT_NULL(sema_resolve_type(sema, func_info->args[i].type)));
    }
    return func_info->sema.type = sema_type_new_func(args, returns);
}

void sema_read_func_info(SemaModule *sema, AstFuncInfo *func_info, bool public) {
    SemaDecl *decl = func_info->sema.decl = sema_decl_new_in_type(
        func_info->name,
        func_info->ext_of ? RET_ON_NULL(sema_resolve_type(sema, func_info->ext_of)) : NULL,
        sema_value_new_runtime(
            sema_value_runtime_new_final(RET_ON_NULL(sema_resolve_func_info_type(sema, func_info))))
    );
    sema_module_push_decl(sema, func_info->name_loc, decl, public);
}
