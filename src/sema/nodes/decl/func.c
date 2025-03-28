#include "func.h"
#include "sema/module.h"
#include "sema/nodes/shared/func_info.h"

void sema_module_read_func(SemaModule *sema, AstFuncDecl *decl, bool is_generic, bool public) {
    if (is_generic) {
        sema_resolve_func_info_type(sema, decl->info);
    } else {
        sema_read_func_info(sema, decl->info, public); 
    }
}
