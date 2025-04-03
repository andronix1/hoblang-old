#include "val_info.h"
#include "ast/shared/val_info.h"
#include "core/not_null.h"
#include "sema/decl.h"
#include "sema/interface/type.h"
#include "sema/interface/value.h"
#include "sema/module.h"
#include "sema/nodes/shared/type.h"
#include "sema/value.h"
#include "core/assert.h"

SemaValue *sema_resolve_val_info(SemaModule *sema, AstValInfo *val_info, SemaValue *initializer, bool public) {
    SemaType *type = val_info->type ? NOT_NULL(sema_resolve_type(sema, val_info->type)) : NULL;
    SemaValue *result = NULL;
    if (!initializer && !type) {
        SEMA_ERROR(val_info->name_loc, "value must be initialized or type must be specified");
        return NULL;
    }
    if (initializer) {
        SemaType *initializer_type = sema_value_is_runtime(initializer);
        if (!initializer_type) {
            SEMA_ERROR(val_info->name_loc, "initializer must be an runtime value");
            return NULL;
        }
        if (!type) {
            type = initializer_type;
        } else {
            if (!sema_types_equals(type, initializer_type)) {
                SEMA_ERROR(val_info->name_loc, "initializer has type {sema::type} depite of {sema::type} was specified as value type", initializer_type, type);
                return NULL;    
            }
        }
    }
    switch (val_info->kind) {
        case AST_VAL_INFO_CONST:
            if (!initializer) {
                SEMA_ERROR(val_info->name_loc, "constants must be initialized");
                return NULL;
            }
            SemaConst *constant = sema_value_is_const(initializer);
            if (!constant) {
                SEMA_ERROR(val_info->name_loc, "initializer is not an constant");
                return NULL; 
            }
            result = sema_value_new_runtime(sema_value_runtime_new_const(constant));
            break;
        case AST_VAL_INFO_FINAL:
            result = sema_value_new_runtime(sema_value_runtime_new_final(type));
            break;
        case AST_VAL_INFO_VAR: 
            result = sema_value_new_runtime(sema_value_runtime_new_var(type));
            break;
    }
    assert(result, "invalid val info type");
    SemaDecl *decl = sema_decl_new(val_info->name, result);
    val_info->sema.decl = decl;
    sema_module_push_decl(sema, val_info->name_loc, decl, public);
    return result;
}
