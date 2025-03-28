#include "generic.h"
#include "sema/module.h"
#include "sema/scope.h"
#include "sema/decl.h"
#include "sema/value.h"
#include "sema/type/type.h"
#include "core/vec.h"
#include "ast/shared/generic.h"

void sema_module_push_generics(SemaModule *sema, AstGenerics *generics) {
    sema_module_push_scope(sema, sema_scope_new(NULL));
    for (size_t i = 0; i < vec_len(generics->params); i++) {
        AstGenericParam *param = &generics->params[i];
        sema_module_push_decl(sema, param->name_loc, sema_decl_new(
            param->name, sema_value_new_comp_time(
                sema_value_comp_time_new_type(sema_type_new_generic(NULL)))
        ), false);
    }
}
