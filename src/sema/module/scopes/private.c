#include "sema/module/scopes/private.h"
#include "sema/module/scopes/impl.h"
#include "sema/module/scopes/scope.h"
#include "sema/module/decls/impl.h"
#include "core/vec.h"

SemaScope *sema_ss_top_scope(SemaScopeStack *stack) {
    return vec_top(stack->scopes);
}

SemaDecl *sema_ss_resolve_decl(SemaScopeStack *stack, SemaType *type, Slice *name) {
    for (ssize_t i = (ssize_t)vec_len(stack->scopes) - 1; i >= 0; i--) {
        SemaScope *scope = &stack->scopes[i];
        for (size_t j = 0; j < vec_len(scope->decls); j++) {
            SemaDecl *decl = scope->decls[j];
            if (decl->in_type == type && slice_eq(&decl->name, name)) {
                return decl;
            }
        }
    }
    return NULL;
}


AstBody *sema_ss_current_body(SemaScopeStack *stack) {
    if (vec_len(stack->scopes) == 0) {
        return NULL;
    }
    return sema_ss_top_scope(stack)->body;
}

void sema_ss_push_body_scope(SemaScopeStack *stack, AstBody *body) {
    AstBody *current_body = sema_ss_current_body(stack);
    if (!body) {
        body = current_body;
    } else {
        body->parent = current_body;
    }
    SemaScope scope = {
        .decls = vec_new(SemaDecl*),
        .body = body
    };
    stack->scopes = vec_push(stack->scopes, &scope);
}

void sema_ss_push_scope(SemaScopeStack *stack) {
    return sema_ss_push_body_scope(stack, NULL);
}

void sema_ss_pop_scope(SemaScopeStack *stack) {
    vec_pop(stack->scopes);
}

SemaDecl *sema_ss_push_decl(SemaScopeStack *stack, SemaDecl *decl) {
    SemaScope *scope = sema_ss_top_scope(stack);
    scope->decls = vec_push(scope->decls, &decl);
    return *vec_top(scope->decls);
}
