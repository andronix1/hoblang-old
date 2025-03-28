#include "behaviour.h"
#include "ast/interface/behaviour.h"
#include "ast/node/decl/behaviour.h"
#include "ast/shared/func_info.h"
#include "core/vec.h"
#include "sema/decl.h"
#include "sema/module.h"
#include "sema/nodes/shared/func_info.h"
#include "sema/scope.h"
#include "sema/type/type.h"
#include "sema/value.h"
#include "sema/behaviour.h"
#include "core/slice/api.h"

bool sema_module_read_behaviour(SemaModule *sema, AstBehaviourDecl *decl, bool is_generic, bool public) {
    SemaScopeStack new_ss = sema_scope_stack_new();
    SemaScopeStack *ss = sema_module_swap_ss(sema, &new_ss);
    sema_module_push_scope(sema, sema_scope_new(NULL));
    sema_module_push_decl(sema, decl->name_loc,
        sema_decl_new(slice_from_cstr("Self"),
            sema_value_new_comp_time(sema_value_comp_time_new_type(sema_type_new_generic(NULL)))),
        false
    );
    bool succeed = true;
    SemaBehaviourRule *rules = vec_new(SemaBehaviourRule);
    for (size_t i = 0; i < vec_len(decl->rules); i++) {
        AstBehaviourRule *rule = &decl->rules[i];
        switch (rule->kind) {
            case AST_NODE_BEHAVIOUR_RULE_FUNC: {
                SemaType *type = sema_resolve_func_info_type(sema, rule->func);
                if (!type) {
                    succeed = false;
                } else {
                    rules = vec_push_dir(rules, sema_behaviour_rule_new_func(rule->func->name, type));
                }
                break;
            }
        }
        Slice name = ast_behaviour_rule_name(rule);
        for (size_t j = i + 1; j < vec_len(decl->rules); j++) {
            AstBehaviourRule *other_rule = &decl->rules[j]; 
            Slice other_name = ast_behaviour_rule_name(other_rule);
            if (slice_eq(&name, &other_name)) {
                succeed = false;
                SEMA_ERROR(other_rule->loc, "rule `{slice}` duplicated", &other_name);
                break;
            }
        }
    }
    if (succeed && !is_generic) {
        sema_module_push_decl(sema, decl->name_loc, sema_decl_new(
            decl->name, sema_value_new_comp_time(sema_value_comp_time_new_behaviour(
                    sema_behaviour_new_rules(rules)))
        ), public);
    }
    sema_module_pop_scope(sema);
    sema_module_swap_ss(sema, ss);
    return succeed;
}
