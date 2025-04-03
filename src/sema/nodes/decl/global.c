#include "global.h"
#include "ast/node/decl/global.h"
#include "sema/nodes/decl/func.h"
#include "sema/nodes/decl/val.h"

void sema_module_read_global_decl(SemaModule *sema, AstGlobalDecl *global_decl, bool public) {
    switch (global_decl->kind) {
        case AST_GLOBAL_DECL_FUNC: sema_module_read_func(sema, global_decl->func_decl, false, public); break;
        case AST_GLOBAL_DECL_VAL: sema_module_read_val_decl(sema, global_decl->val_decl, public); break;
    }
}

void sema_module_analyze_global_decl(SemaModule *sema, AstGlobalDecl *global_decl) {
    switch (global_decl->kind) {
        case AST_GLOBAL_DECL_FUNC: sema_analyze_func_decl(sema, global_decl->func_decl); break;
        case AST_GLOBAL_DECL_VAL: break;
    }
}
