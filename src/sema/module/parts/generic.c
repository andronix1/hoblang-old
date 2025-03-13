#include "ast/private/generic.h"
#include "ast/private/path.h"
#include "core/vec.h"
#include "sema/module/behaviour/impl.h"
#include "sema/module/behaviour/private.h"
#include "sema/module/impl.h"
#include "sema/module/private.h"
#include "sema/type/private.h"
#include "sema/value/api.h"
#include "sema/value/private.h"
#include "sema/module/parts/path.h"

SemaType *sema_analyze_generic_rules(SemaModule *sema, AstGenericRules *rules) {
    SemaBehaviour **behaviours = vec_new(SemaBehaviour*);
    for (size_t i = 0; i < vec_len(rules->behaviours); i++) {
        AstPath *path = rules->behaviours[i];
        SemaValue *value = sema_resolve_path(sema, NULL, path);
        if (!value) {
            return NULL;
        }
        if (!sema_value_is_behaviour(value)) {
            SEMA_ERROR(path->segments[0].loc, "non-behaviour in generic rules");
            continue;
        }
        behaviours = vec_push(behaviours, &value->behaviour);
    }
    return sema_type_new_generic(sema_behaviour_new_list(behaviours));
}

SemaType *sema_analyze_generic_param(SemaModule *sema, AstGenericParam *param) {
    return sema_analyze_generic_rules(sema, &param->rules);
}
