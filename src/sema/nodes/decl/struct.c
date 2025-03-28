#include "struct.h"
#include "ast/node/decl/struct.h"
#include "core/slice/api.h"
#include "core/vec.h"
#include "sema/decl.h"
#include "sema/module.h"
#include "sema/type/type.h"
#include "sema/nodes/shared/type.h"
#include "sema/value.h"

bool sema_module_read_struct(SemaModule *sema, AstStructDecl *decl, bool is_generic, bool public) {
    bool succeed = true;
    for (size_t i = 0; i < vec_len(decl->fields); i++) {
        AstStructDeclField *field = &decl->fields[i];
        if (!sema_resolve_type(sema, field->type)) {
            succeed = false;
        }
        for (size_t j = i + 1; j < vec_len(decl->fields); j++) {
            AstStructDeclField *other_field = &decl->fields[j];
            if (slice_eq(&field->name, &other_field->name)) {
                SEMA_ERROR(other_field->name_loc, "field `{slice}` duplicated", &field->name);
            }
        }
    }
    if (succeed && !is_generic) {
        sema_module_push_decl(sema, decl->name_loc, sema_decl_new(decl->name, sema_value_new_comp_time(sema_value_comp_time_new_type(sema_type_new_struct(decl)))), public);
    }
    return succeed;
}
