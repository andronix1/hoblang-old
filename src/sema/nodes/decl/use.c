#include "use.h"
#include "core/not_null.h"
#include "sema/decl.h"
#include "sema/module.h"
#include "sema/nodes/shared/path.h"

void sema_module_read_use(SemaModule *sema, AstUse *use, bool public) {
    SemaDecl *decl = RET_ON_NULL(sema_resolve_decl_path(sema, sema, use->path));
    sema_module_push_decl(sema, ast_path_loc(use->path), decl, public);
}
