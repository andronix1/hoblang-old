#include "from_use.h"
#include "core/not_null.h"
#include "core/vec.h"
#include "sema/decl.h"
#include "sema/interface/value.h"
#include "sema/module.h"
#include "sema/nodes/shared/path.h"

void sema_module_read_from_use(SemaModule *sema, AstFromUse *from_use, bool public) {
    SemaModule *module = sema_value_is_module(RET_ON_NULL(sema_resolve_path(sema, from_use->from)));
    if (!module) {
        SEMA_ERROR(ast_path_loc(from_use->from), "path be a module");
        return;
    }
    switch (from_use->kind) {
        case AST_FROM_USE_LIST:
            for (size_t i = 0; i < vec_len(from_use->list); i++) {
                AstPath *path = from_use->list[i];
                SemaValue *value = sema_resolve_path(module, path);
                if (value) {
                    sema_module_push_decl(sema, ast_path_loc(path),
                            sema_decl_new(ast_path_name(path), value), public);
                }
            }
            break;
        case AST_FROM_USE_ALL: SEMA_ERROR(from_use->all_loc, "NIY"); break;
    }
}
