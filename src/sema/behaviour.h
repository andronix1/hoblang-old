#pragma once

#include <malloc.h>
#include "core/slice/slice.h"
#include "sema/interface/type.h"
#include "sema/interface/behaviour.h"
#include "sema/type/type.h"
#include "core/assert.h"

typedef enum {
    SEMA_BEHAVIOUR_RULE_FUNC
} SemaBehaviourRuleKind;

typedef struct {
    SemaType *type;
    Slice name;
} SemaBehaviourRuleFunc;

typedef struct {
    SemaBehaviourRuleKind kind;
    union {
        SemaBehaviourRuleFunc func;
    };
} SemaBehaviourRule;

typedef enum {
    SEMA_BEHAVIOUR_LIST,
    SEMA_BEHAVIOUR_RULES,
} SemaBehaviourKind;

typedef struct SemaBehaviour {
    SemaBehaviourKind kind;
    union {
        SemaBehaviourRule *rules;
        SemaBehaviour **list;
    };
} SemaBehaviour;

static inline SemaBehaviourRule sema_behaviour_rule_new_func(Slice name, SemaType *type) {
    assert(type->kind == SEMA_TYPE_FUNCTION, "passed type is not a function");
    SemaBehaviourRule result = {
        .kind = SEMA_BEHAVIOUR_RULE_FUNC,
        .func = {
            .type = type,
            .name = name
        }
    };
    return result;
}

static inline SemaBehaviour *sema_behaviour_new_rules(SemaBehaviourRule *rules) {
    SemaBehaviour *result = malloc(sizeof(SemaBehaviour));
    result->kind = SEMA_BEHAVIOUR_RULES;
    result->rules = rules;
    return result;
}

static inline SemaBehaviour *sema_behaviour_new_list(SemaBehaviour **list) {
    SemaBehaviour *result = malloc(sizeof(SemaBehaviour));
    result->kind = SEMA_BEHAVIOUR_LIST;
    result->list = list;
    return result;
}
