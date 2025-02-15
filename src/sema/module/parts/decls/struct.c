#include "sema/module/private.h"
#include "ast/private/module_node.h"
#include "sema/module/parts/decls/struct/api.h"
#include "sema/module/parts/decls/struct/impl.h"

SemaStructMember *sema_get_struct_member(SemaModule *sema, AstStructDef *struct_def, Slice *name) {
    for (size_t i = 0; i < vec_len(struct_def->members); i++) {
        AstStructMember *member = &struct_def->members[i];
        if (slice_eq(&member->name, name)) {
            return sema_struct_member_field(i);
        }
    }
    for (size_t i = 0; i < vec_len(struct_def->ext_funcs); i++) {
        SemaStructExtFunc *ext_func = &struct_def->ext_funcs[i];
        if (slice_eq(&ext_func->name, name)) {
            return sema_struct_member_ext_func(ext_func->decl);
        }
    }
    sema_err("struct {slice} has not a member {slice}", &struct_def->name, name);
    return NULL;
}