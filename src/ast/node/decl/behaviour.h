#pragma once

#include "ast/interface/func_info.h"
#include "core/location.h"
#include "core/slice/slice.h"

typedef enum {
    AST_NODE_BEHAVIOUR_RULE_FUNC
} AstBehaviourRuleKind;

typedef struct AstBehaviourRule {
    AstBehaviourRuleKind kind;
    FileLocation loc;
    union {
        AstFuncInfo *func;
    };
} AstBehaviourRule;

typedef struct {
    Slice name;
    FileLocation name_loc;
    AstBehaviourRule *rules;
} AstBehaviourDecl;

static inline AstBehaviourRule ast_behaviour_rule_new_func(FileLocation loc, AstFuncInfo *info) {
    AstBehaviourRule result = {
        .func = info,
        .loc = loc
    };
    return result;
}

static inline AstBehaviourDecl ast_behaviour_decl_new(Slice name, FileLocation name_loc, AstBehaviourRule *rules) {
    AstBehaviourDecl result = {
        .name = name,
        .name_loc = name_loc,
        .rules = rules 
    };
    return result;
}

