#include "ast/private/module_node.h"
#include "sema/module/parts/decls/struct/api.h"

ssize_t sema_get_struct_member_id(SemaModule *sema, FileLocation at, AstStructDef *struct_def, Slice *name) {
    for (size_t i = 0; i < vec_len(struct_def->members); i++) {
        AstStructMember *member = &struct_def->members[i];
        if (slice_eq(&member->name, name)) {
            if (!member->public && sema != struct_def->module) {
                return -1;
            }
            return i;
        }
    }
    return -1;
}
