#include "sema/module/behaviour/table/impl.h"
#include "sema/module/behaviour/table/api.h"
#include "core/vec.h"
#include "sema/module/decls/impl.h"
#include "sema/module/private.h"
#include "sema/value/api.h"

SemaBehaviourTableElement *sema_type_behaves_as_decl(SemaModule *sema, FileLocation at, SemaType *type, SemaBehaviour *behaviour) {
    SemaBehaviourTableElement *elements = vec_new(SemaBehaviourTableElement);
    for (size_t i = 0; i < vec_len(behaviour->decl.rules); i++) {
        SemaBehaviourRule *rule = &behaviour->decl.rules[i];
        SemaBehaviourTableElement element;
        switch (rule->type) {
            case SEMA_BEHAVIOUR_RULE_FUN: {
                SemaDecl *decl = element.decl = sema_module_resolve_ext_func(sema, &rule->func.name, type);
                if (!decl) {
                    SEMA_ERROR(at, "no ext func `{slice}` found", &rule->func.name);
                    return NULL;
                }
                behaviour->decl.self->generic.replace = type;
                SemaType *found_type = sema_value_typeof(decl->value);
                if (!sema_types_equals(found_type, rule->func.type)) {
                    SEMA_ERROR(at, "invalid `{slice}` type {sema::type}. Expected {sema::type}", &rule->func.name, found_type, rule->func.type);
                    return NULL;
                }
                break;
            }
        }
        elements = vec_push(elements, &element);
    }
    
    return elements;
}

SemaBehaviourTable *sema_type_build_behaviour_table(SemaModule *sema, FileLocation at, SemaType *type, SemaBehaviour *behaviour) {
    SemaBehaviourTable *table = malloc(sizeof(SemaBehaviourTable));
    table->of = type;
    if (!behaviour) {
        return table;
    }
    table->kind = (int)behaviour->type;
    switch (behaviour->type) {
        case SEMA_BEHAVIOUR_DECL:
            table->decl = sema_type_behaves_as_decl(sema, at, type, behaviour);
            return table;
        case SEMA_BEHAVIOUR_LIST: {
            SemaBehaviourTable **list = vec_new(SemaBehaviourTable*);
            bool behaves = true;
            for (size_t i = 0; i < vec_len(behaviour->list); i++) {
                SemaBehaviourTable *table = sema_type_build_behaviour_table(sema, at, type, behaviour->list[i]);
                if (!table) {
                    behaves = false;
                }
                list = vec_push(list, &table);
            }
            if (!behaves) {
                return NULL;
            }
            table->list = list;
            return table;
        }
    }
    assert(0, "unreachable");
}


