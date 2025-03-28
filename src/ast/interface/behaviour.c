#include "behaviour.h"
#include "ast/node/decl/behaviour.h"
#include "ast/shared/func_info.h"
#include "core/assert.h"

Slice ast_behaviour_rule_name(AstBehaviourRule *rule) {
    switch (rule->kind) {
        case AST_NODE_BEHAVIOUR_RULE_FUNC: return rule->func->name;
    }
    assert(0, "invalid behaviour rule");
}
