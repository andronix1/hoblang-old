#include "sema/module/parts/decls/behaviour.h"
#include "ast/private/decls/behaviour.h"
#include "ast/private/func_info.h"
#include "core/vec.h"
#include "sema/module/behaviour/impl.h"
#include "sema/module/decls/api.h"
#include "sema/module/parts/type.h"
#include "sema/module/private.h"
#include "sema/type/private.h"
#include "sema/value/private.h"
#include <stdlib.h>

SemaBehaviour *sema_analyze_behaviour_decl(SemaModule *sema, AstDeclBehaviour *decl) {
    // TODO: check duplicates
    sema_module_push_scope(sema);
    SemaType *type = sema_type_new_generic(NULL);
    type->generic.decl = sema_module_push_scope_decl(sema, decl->loc, sema_decl_new(
        slice_from_cstr("Self"),
        sema_value_type(type)
    ));
    SemaBehaviour *behaviour = malloc(sizeof(SemaBehaviour));
    for (size_t i = 0; i < vec_len(decl->rules); i++) {
        SemaBehaviourRule sema_rule;
        AstBehaviourRule *rule = &decl->rules[i];
        switch (rule->type) {
            case AST_BEHAVIOUR_RULE_FUNC: {
                AstBehaviourRuleFunc *func = &rule->func;
                sema_rule.type = SEMA_BEHAVIOUR_RULE_FUN;
                SemaBehaviourRuleFunc *sema_func = &sema_rule.func;
                if (func->in_type) {
                    sema_func->in_type = sema_ast_type(sema, func->in_type);
                }
                // TODO: check duplicates and move in func
                sema_func->function.args = vec_new(SemaType*);
                for (size_t i = 0; i < vec_len(func->info.args); i++) {
                    AstFuncArg *arg = &func->info.args[i];
                    SemaType *arg_type = sema_ast_type(sema, arg->type);
                    sema_func->function.args = vec_push(sema_func->function.args, &arg_type);
                }
                sema_func->function.returning = sema_ast_type(sema, func->info.returning);
            }
        }
    }
    sema_module_pop_scope(sema);
    return behaviour;
}
