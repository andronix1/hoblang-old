#include "core/vec.h"
#include "sema/module/behaviour/impl.h"
#include "sema/module/behaviour/api.h"
#include "sema/module/behaviour/table/path.h"
#include "sema/type/private.h"
#include "sema/value/private.h"

SemaValue *_sema_behaviour_resolve(SemaBehaviour *behaviour, Slice *name, SemaBehaviourTablePath *result);

SemaValue *sema_behaviour_list_resolve(SemaBehaviour *behaviour, Slice *name, SemaBehaviourTablePath *result) {
    for (size_t i = 0; i < vec_len(behaviour->list); i++) {
        SemaValue *value = _sema_behaviour_resolve(behaviour->list[i], name, result);
        if (value) {
            SemaBehaviourTablePathElement element = {
                .kind = SEMA_BEHAVIOUR_LIST,
                .idx = i
            };
            result->path = vec_push(result->path, &element);
            return value;
        }
    }
    return NULL;
}

SemaValue *sema_behaviour_decl_resolve(SemaBehaviour *behaviour, Slice *name, SemaBehaviourTablePath *result) {
    for (size_t i = 0; i < vec_len(behaviour->decl.rules); i++) {
        SemaBehaviourRule *rule = &behaviour->decl.rules[i];
        switch (rule->type) {
            case SEMA_BEHAVIOUR_RULE_FUN:
                if (!slice_eq(&rule->func.name, name)) {
                    break;
                }
                SemaBehaviourTablePathElement element = {
                    .kind = SEMA_BEHAVIOUR_DECL,
                    .idx = i
                };
                result->path = vec_push(result->path, &element);
                return sema_value_final(sema_type_new_func(
                    rule->func.function.returning,
                    rule->func.function.args
                ));
        }
    }
    return NULL;
}

SemaValue *sema_behaviour_resolve(SemaBehaviour *behaviour, Slice *name, SemaBehaviourTablePath *result) {
    if (!behaviour) {
        return NULL;
    }
    result->path = vec_new(SemaBehaviourTablePathElement);
    SemaValue *value = _sema_behaviour_resolve(behaviour, name, result);
    size_t len = vec_len(result->path);
    for (size_t i = 0; i < len / 2; i++) {
        SemaBehaviourTablePathElement tmp;
        memcpy(&tmp, &result->path[i], sizeof(SemaBehaviourTablePathElement));
        memcpy(&result->path[i], &result->path[len - i - 1], sizeof(SemaBehaviourTablePathElement));
        memcpy(&result->path[len - i - 1], &tmp, sizeof(SemaBehaviourTablePathElement));
    }
    return value;
}

SemaValue *_sema_behaviour_resolve(SemaBehaviour *behaviour, Slice *name, SemaBehaviourTablePath *result) {
    switch (behaviour->type) {
        case SEMA_BEHAVIOUR_DECL:
            return sema_behaviour_decl_resolve(behaviour, name, result);
        case SEMA_BEHAVIOUR_LIST:
            return sema_behaviour_list_resolve(behaviour, name, result);
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
