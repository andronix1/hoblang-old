#include "global.h"
#include "ast/shared/global.h"
#include "sema/module.h"
#include "sema/nodes/shared/func_info.h"
#include "sema/nodes/shared/val_info.h"
#include "ast/shared/val_info.h"

void sema_module_read_global_decl_info(SemaModule *sema, AstGlobalDeclInfo *info, bool public) {
    switch (info->kind) {
        case AST_GLOBAL_DECL_INFO_FUNC: sema_read_func_info(sema, info->func, public); break;
        case AST_GLOBAL_DECL_INFO_VAL:
            if (info->val->kind == AST_VAL_INFO_CONST) {
                SEMA_ERROR(info->val->name_loc, "external values cannot be a constant");
                return;
            }
            sema_resolve_val_info(sema, info->val, NULL, public);
            break;
    }
}
