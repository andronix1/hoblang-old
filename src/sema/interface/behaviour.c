#include "behaviour.h"
#include "core/vec.h"
#include "sema/behaviour.h"
#include "sema/module.h"

bool sema_type_behaves_like(SemaModule *sema, SemaType *type, SemaBehaviour *behaviour) {
    switch (behaviour->kind) {
        case SEMA_BEHAVIOUR_RULES: {
            for (size_t i = 0; i < vec_len(behaviour->rules); i++) {
                SemaBehaviourRule *rule = &behaviour->rules[i];
                switch (rule->kind) {
                    case SEMA_BEHAVIOUR_RULE_FUNC:
                        if (!sema_module_resolve_decl(sema, &rule->func.name, type)) {
                            return false;
                        }
                        break;
                }
            }
            return true;
        }
        case SEMA_BEHAVIOUR_LIST:
            for (size_t i = 0; i < vec_len(behaviour->list); i++) {
                if (!sema_type_behaves_like(sema, type, behaviour->list[i])) {
                    return false;
                }
            }
            return true;
    }
    assert(0, "invalid behaviour");
}
