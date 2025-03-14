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

bool sema_type_behaves_as_decl(SemaModule *sema, FileLocation at, SemaType *type, SemaBehaviour *behaviour) {
    for (size_t i = 0; i < vec_len(behaviour->decl.rules); i++) {
        SemaBehaviourRule *rule = &behaviour->decl.rules[i];
        switch (rule->type) {
            case SEMA_BEHAVIOUR_RULE_FUN: {
                SemaDecl *decl = sema_module_resolve_ext_func(sema, &rule->func.name, type);
                if (!decl) {
                    SEMA_ERROR(at, "no ext func `{slice}` found", &rule->func.name);
                    return false;
                }
                behaviour->decl.self->generic.replace = type;
                SemaType *found_type = sema_value_typeof(decl->value);
                if (!sema_types_equals(found_type, rule->func.type)) {
                    SEMA_ERROR(at, "invalid `{slice}` type {sema::type}. Expected {sema::type}", &rule->func.name, found_type, rule->func.type);
                    return false;
                }
                break;
            }
        }
    }
    
    return true;
}

bool sema_type_behaves_as(SemaModule *sema, FileLocation at, SemaType *type, SemaBehaviour *behaviour) {
    if (!behaviour) {
        return true;
    }
    switch (behaviour->type) {
        case SEMA_BEHAVIOUR_DECL:
            return sema_type_behaves_as_decl(sema, at, type, behaviour);
        case SEMA_BEHAVIOUR_LIST: {
            bool behaves = true;
            for (size_t i = 0; i < vec_len(behaviour->list); i++) {
                if (!sema_type_behaves_as(sema, at, type, behaviour->list[i])) {
                    behaves = false;
                }
            }
            return behaves;
        }
    }
    assert(0, "unreachable");
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
                sema_func->function.args = vec_push(sema_func->function.args, &sema_func->in_type);
                for (size_t i = 0; i < vec_len(func->info.args); i++) {
                    AstFuncArg *arg = &func->info.args[i];
                    SemaType *arg_type = sema_ast_type(sema, arg->type);
                    sema_func->function.args = vec_push(sema_func->function.args, &arg_type);
                }
                sema_func->function.returning = sema_ast_type(sema, func->info.returning);
                sema_func->type = sema_type_new_func(
                    sema_func->function.returning,
                    sema_func->function.args
                );
                behaviour->decl.rules = vec_push(behaviour->decl.rules, &sema_rule);
                break;
            }
        }
    }
    sema_module_pop_scope(sema);
    return behaviour;
}
