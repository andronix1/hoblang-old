#include "type.h"
#include "core/not_null.h"
#include "sema/decl.h"
#include "sema/module.h"
#include "sema/nodes/shared/type.h"
#include "sema/value.h"

bool sema_module_read_type_alias(SemaModule *sema, AstTypeAlias *decl, bool is_generic, bool public) {
    SemaType *type = NOT_NULL(sema_resolve_type(sema, decl->type));
    if (!is_generic) {
        sema_module_push_decl(sema, decl->name_loc, sema_decl_new(decl->name, sema_value_new_comp_time(sema_value_comp_time_new_type(type))), public);
    }
    return true;
}
