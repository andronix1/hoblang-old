#include "scope.h"
#include "core/location.h"
#include "core/slice/api.h"
#include "core/slice/slice.h"
#include "core/vec.h"
#include "sema/interface/decl.h"
#include "sema/decl.h"
#include "sema/interface/value.h"
#include "sema/loop.h"
#include "sema/module.h"
#include "sema/type/matches.h"
#include "sema/value.h"
#include <stdlib.h>

SemaScope *sema_scope_new(SemaType *returns) {
    SemaScope *result = malloc(sizeof(SemaScope));
    result->decls = vec_new(SemaDecl*);
    result->returns = returns;
    return result;
}

SemaDecl *sema_scope_resolve_decl(SemaModule *sema, SemaScope *scope, Slice *name, SemaType *in_type) {
    for (size_t i = 0; i < vec_len(scope->decls); i++) {
        SemaDecl *decl = scope->decls[i];
        if ((decl->in_type == NULL) != (in_type == NULL)) {
            continue;
        }
        if (!slice_eq(&decl->name, name)) {
            continue;
        }
        if (in_type == NULL && decl->in_type == NULL) {
            return decl;
        }
        SemaTypeMatches *matches = sema_type_matches_new();
        if (sema_type_matches(sema, in_type, decl->in_type, matches)) {
           if (vec_len(matches->matches) > 0) {
                return sema_decl_new(
                    decl->name,
                    sema_value_new_runtime(sema_value_runtime_new_final(
                            sema_type_generate(sema_value_is_runtime(decl->value), matches)))
                );
            }
            return decl;
        }
    }
    return NULL;
}

void sema_scope_push_decl(SemaModule *sema, FileLocation at, SemaScope *scope, SemaDecl *decl) {
    if (sema_scope_resolve_decl(sema, scope, &decl->name, decl->in_type)) {
        SEMA_ERROR(at, "duplicates decl `{slice}`", &decl->name);
        return;
    }
    scope->decls = vec_push(scope->decls, &decl);
}

SemaScopeStack sema_scope_stack_new() {
    SemaScopeStack result = {
        .scopes = vec_new(SemaScope*),
        .loops = vec_new(SemaLoop*)
    };
    return result;
}

void sema_ss_push_loop(SemaScopeStack *ss, SemaLoop *loop) {
    ss->loops = vec_push_dir(ss->loops, loop);
}

void sema_ss_pop_loop(SemaScopeStack *ss) {
    vec_pop(ss->loops);
}

SemaLoop *sema_ss_resolve_loop(SemaScopeStack *ss) {
    return vec_len(ss->loops) ? *vec_top(ss->loops) : NULL;
}

SemaLoop *sema_ss_resolve_named_loop(SemaScopeStack *ss, Slice *name) {
    for (ssize_t i = vec_len(ss->loops) - 1; i >= 0; i--) {
        SemaLoop *loop = ss->loops[i];
        if (loop->is_labelled && slice_eq(name, &loop->label)) {
            return loop;
        }
    }
    return NULL;
}

void sema_ss_push_scope(SemaScopeStack *ss, SemaScope *scope) {
    ss->scopes = vec_push(ss->scopes, &scope);
}

void sema_ss_pop_scope(SemaScopeStack *ss) {
    vec_pop(ss->scopes);
}

SemaScope *sema_ss_top(SemaScopeStack *ss) {
    return *vec_top(ss->scopes);
}

SemaDecl *sema_ss_resolve_decl(SemaModule *sema, SemaScopeStack *ss, Slice *name, SemaType *in_type) {
    for (ssize_t i = vec_len(ss->scopes) - 1; i >= 0; i--) {
        SemaDecl *decl = sema_scope_resolve_decl(sema, ss->scopes[i], name, in_type);
        if (decl) {
            return decl;
        }
    }
    return false;
}

SemaType *sema_ss_returns(SemaScopeStack *ss) {
    for (ssize_t i = vec_len(ss->scopes) - 1; i >= 0; i--) {
        if (ss->scopes[i]->returns) {
            return ss->scopes[i]->returns;
        }
    }
    return NULL;
}
