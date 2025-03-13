#pragma once

#include "core/slice/slice.h"
#include "sema/module/decls/decls.h"
#include "sema/type.h"
#include "sema/type/func.h"

typedef enum {
    SEMA_BEHAVIOUR_RULE_FUN
} SemaBehaviourRuleType;

typedef struct SemaBehaviourRuleFunc {
    Slice name;
    SemaType *in_type;
    SemaFunction function;
} SemaBehaviourRuleFunc;

typedef struct SemaBehaviourRule {
    SemaBehaviourRuleType type;

    union {
        SemaBehaviourRuleFunc func;
    };
} SemaBehaviourRule;

typedef struct SemaBehaviour {
    SemaBehaviourRule *rules;
} SemaBehaviour;
