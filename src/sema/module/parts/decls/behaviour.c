#include "sema/module/parts/decls/behaviour.h"
#include "ast/private/decls/behaviour.h"
#include "ast/private/func_info.h"
#include "core/location.h"
#include "core/slice/api.h"
#include "core/vec.h"
#include "sema/module/behaviour/impl.h"
#include "sema/module/decls/api.h"
#include "sema/module/parts/type.h"
#include "sema/module/private.h"
#include "sema/type/api.h"
#include "sema/type/private.h"
#include "sema/value/api.h"
#include "sema/value/private.h"
#include "sema/module/decls/impl.h"
#include <stdlib.h>

bool sema_type_behaves_as(SemaModule *sema, FileLocation at, SemaType *type, SemaBehaviour *behaviour) {
    if (behaviour == NULL) {
        return true;
    }
    for (size_t i = 0; i < vec_len(behaviour->decl.rules); i++) {
        SemaBehaviourRule *rule = &behaviour->decl.rules[i];
        switch (rule->type) {
            case SEMA_BEHAVIOUR_RULE_FUN:
                SemaDecl *decl = sema_module_resolve_scope_decl(sema, &rule->func.name);
                if (!decl) {
                    return false;
                }
                SemaType *type = sema_value_typeof(decl->value);
                if (!sema_value_is_final(decl->value)) {
                    SEMA_ERROR(at, "expected function found {sema::type}", type);
                    return false;
                }
                if (!sema_types_equals(type, rule->func.type)) {
                    SEMA_ERROR(at, "unresolved rule `{slice}`", &rule->func.name);
                    return false;
                }
                break;
        }
    }
    
    return true;
}

SemaBehaviour *sema_analyze_behaviour_decl(SemaModule *sema, AstDeclBehaviour *decl) {
    // TODO: check duplicates
    sema_module_push_scope(sema);
    SemaType *self_type = sema_type_new_generic(NULL);
    sema_module_push_scope_decl(sema, decl->loc, sema_decl_new(
        slice_from_cstr("Self"),
        sema_value_type(self_type)
    ));
    SemaBehaviour *behaviour = malloc(sizeof(SemaBehaviour));
    behaviour->type = SEMA_BEHAVIOUR_DECL;
    behaviour->decl.rules = vec_new(SemaBehaviourRule);
    behaviour->decl.self = self_type;
    for (size_t i = 0; i < vec_len(decl->rules); i++) {
        SemaBehaviourRule sema_rule;
        AstBehaviourRule *rule = &decl->rules[i];
        switch (rule->type) {
            case AST_BEHAVIOUR_RULE_FUNC: {
                AstBehaviourRuleFunc *func = &rule->func;
                sema_rule.type = SEMA_BEHAVIOUR_RULE_FUN;
                sema_rule.func.name = func->name;
                SemaBehaviourRuleFunc *sema_func = &sema_rule.func;
                    sema_func->in_type = sema_ast_type(sema, func->in_type);
                if (
                    !sema_types_equals(sema_func->in_type, self_type) &&
                    !(
                        sema_func->in_type->type == SEMA_TYPE_POINTER &&
                        sema_types_equals(sema_func->in_type->ptr_to, self_type)
                     )
                ) {
                    SEMA_ERROR(rule->loc, "behaviour's functions must be in Self or *Self");
                }
                // TODO: check duplicates and move in func
                sema_func->function.args = vec_new(SemaType*);
                for (size_t i = 0; i < vec_len(func->info.args); i++) {
                    AstFuncArg *arg = &func->info.args[i];
                    SemaType *arg_type = sema_ast_type(sema, arg->type);
                    sema_func->function.args = vec_push(sema_func->function.args, &arg_type);
                }
                sema_func->function.returning = sema_ast_type(sema, func->info.returning);
                behaviour->decl.rules = vec_push(behaviour->decl.rules, &sema_rule);
                break;
            }
        }
    }
    sema_module_pop_scope(sema);
    return behaviour;
}
