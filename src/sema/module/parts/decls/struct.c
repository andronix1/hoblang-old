#include "sema/module/private.h"
#include "sema/type/private.h"
#include "sema/module/decls/impl.h"
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
    // TODO: remove from struct
    SemaScopeDecl *decl = sema_module_resolve_ext_func(sema, name, sema_type_new_struct(struct_def));
    if (decl) {
        return sema_struct_member_ext_func(&decl->value_decl, false);
    }
    SemaScopeDecl *ptr_decl = sema_module_resolve_ext_func(sema, name, sema_type_new_pointer(sema_type_new_struct(struct_def)));
    if (ptr_decl) {
        return sema_struct_member_ext_func(&ptr_decl->value_decl, true);
    }
    sema_err("struct {slice} has not a member {slice}", &struct_def->name, name);
    return NULL;
}
