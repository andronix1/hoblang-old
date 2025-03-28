#include "external.h"
#include "sema/module.h"
#include "sema/nodes/shared/func_info.h"
#include "sema/nodes/shared/val_info.h"
#include "ast/shared/val_info.h"

void sema_module_read_external(SemaModule *sema, AstExternalDecl *decl, bool public) {
    switch (decl->kind) {
        case AST_EXTERNAL_DECL_FUNC: sema_read_func_info(sema, decl->func, public); break;
        case AST_EXTERNAL_DECL_VAL:
            if (decl->val->kind == AST_VAL_INFO_CONST) {
                SEMA_ERROR(decl->val->name_loc, "external values cannot be a constant");
                return;
            }
            sema_resolve_val_info(sema, decl->val, NULL, public);
            break;
    }
}
