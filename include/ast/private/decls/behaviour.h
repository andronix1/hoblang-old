#pragma once

#include "ast/private/func_info.h"
#include "ast/private/type.h"
#include "core/location.h"
#include "core/slice/slice.h"
#include "sema/module/decls/decls.h"

typedef enum {
    AST_BEHAVIOUR_RULE_FUNC
} AstBehaviourRuleType;

typedef struct AstBehaviourRuleFunc {
    Slice name;
    AstType *in_type;
    AstFuncTypeInfo info;
} AstBehaviourRuleFunc;

typedef struct AstBehaviourRule {
    AstBehaviourRuleType type;
    FileLocation loc;

    union {
        AstBehaviourRuleFunc func;
    };
} AstBehaviourRule;

typedef struct AstDeclBehaviour {
    Slice name;
    AstBehaviourRule *rules;
    FileLocation loc;

    struct {
        SemaDecl *decl;
    } sema;
} AstDeclBehaviour;
