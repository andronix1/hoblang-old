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

    SemaType *type;
} SemaBehaviourRuleFunc;

typedef struct SemaBehaviourRule {
    SemaBehaviourRuleType type;

    union {
        SemaBehaviourRuleFunc func;
    };
} SemaBehaviourRule;

typedef enum {
    SEMA_BEHAVIOUR_DECL,
    SEMA_BEHAVIOUR_LIST
} SemaBehaviourKind;

typedef struct SemaBehaviourDecl {
    SemaBehaviourRule *rules;
    SemaType *self;
} SemaBehaviourDecl;

typedef struct SemaBehaviour {
    SemaBehaviourKind type;

    union {
        SemaBehaviourDecl decl;
        struct SemaBehaviour **list;
    };
} SemaBehaviour;
