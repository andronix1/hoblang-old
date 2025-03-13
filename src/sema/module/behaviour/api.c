#include "core/vec.h"
#include "sema/module/behaviour/impl.h"
#include "sema/module/behaviour/api.h"
#include "sema/type/private.h"
#include "sema/value.h"
#include "sema/value/private.h"

SemaValue *sema_behaviour_list_resolve(SemaBehaviour *behaviour, Slice *name) {
    for (size_t i = 0; i < vec_len(behaviour->list); i++) {
        SemaValue *value = sema_behaviour_resolve(behaviour->list[i], name);
        if (value) {
            return value;
        }
    }
    return NULL;
}

SemaValue *sema_behaviour_decl_resolve(SemaBehaviour *behaviour, Slice *name) {
    for (size_t i = 0; i < vec_len(behaviour->decl.rules); i++) {
        SemaBehaviourRule *rule = &behaviour->decl.rules[i];
        switch (rule->type) {
            case SEMA_BEHAVIOUR_RULE_FUN:
                if (!slice_eq(&rule->func.name, name)) {
                    break;
                }
                return sema_value_final(sema_type_new_func(
                    rule->func.function.returning,
                    rule->func.function.args
                ));
        }
    }
    return NULL;
}

SemaValue *sema_behaviour_resolve(SemaBehaviour *behaviour, Slice *name) {
    if (!behaviour) {
        return NULL;
    }
    switch (behaviour->type) {
        case SEMA_BEHAVIOUR_DECL:
            return sema_behaviour_decl_resolve(behaviour, name);
        case SEMA_BEHAVIOUR_LIST:
            return sema_behaviour_list_resolve(behaviour, name);
    }
    assert(0, "unreachable");
}

SemaType *sema_behaviour_swap_self_type(SemaBehaviour *behaviour, SemaType *new) {
    if (!behaviour) {
        return NULL;
    }
    switch (behaviour->type) {
        case SEMA_BEHAVIOUR_DECL: {
            SemaType *old = behaviour->decl.self;
            behaviour->decl.self->generic.replace = new;
            return old;
        }
        case SEMA_BEHAVIOUR_LIST: {
            SemaType *old = NULL;
            for (size_t i = 0; i < vec_len(behaviour->list); i++) {
                old = sema_behaviour_swap_self_type(behaviour->list[i], new);
            }
            return old;
        }
    }
    assert(0, "unreachable");

}
