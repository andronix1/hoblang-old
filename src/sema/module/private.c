#include "sema/module/impl.h"
#include "core/vec.h"
#include "sema/type/private.h"
#include "sema/module/private.h"
#include "sema/module/decls/impl.h"
#include "sema/type/api.h"

SemaScopeDecl *sema_scope_decl_new_type(Slice name, SemaType *sema_type) {
    SemaScopeDecl *result = malloc(sizeof(SemaScopeDecl));
    result->type = SEMA_SCOPE_DECL_TYPE;
    result->in_type = NULL;
    result->name = name;
    result->sema_type = sema_type;
    return result;
}

SemaScopeDecl *sema_scope_decl_new_in_type_value(Slice name, SemaType *in_type, SemaType *type, bool constant) {
    SemaScopeDecl *result = malloc(sizeof(SemaScopeDecl));
    result->type = SEMA_SCOPE_DECL_VALUE;
    result->in_type = in_type;
    result->name = name;
    result->value_decl.type = type;
    result->value_decl.constant = constant;
    result->value_decl.llvm_value = NULL;
    return result;
}

SemaScopeDecl *sema_scope_decl_new_value(Slice name, SemaType *type, bool constant) {
    SemaScopeDecl *result = malloc(sizeof(SemaScopeDecl));
    result->type = SEMA_SCOPE_DECL_VALUE;
    result->in_type = NULL;
    result->name = name;
    result->value_decl.type = type;
    result->value_decl.constant = constant;
    result->value_decl.llvm_value = NULL;
    return result;
}

SemaScopeDecl *sema_scope_decl_new_module(Slice name, SemaModule *module) {
    SemaScopeDecl *result = malloc(sizeof(SemaScopeDecl));
    result->type = SEMA_SCOPE_DECL_MODULE;
    result->in_type = NULL;
    result->name = name;
    result->module = module;
    return result;
}

SemaScopeDecl *sema_module_resolve_ext_func(SemaModule *sema, Slice *name, SemaType *type) {
    for (ssize_t i = (ssize_t)vec_len(sema->scopes) - 1; i >= 0; i--) {
        SemaScope *scope = &sema->scopes[i];
        for (size_t j = 0; j < vec_len(scope->decls); j++) {
            SemaScopeDecl *decl = scope->decls[j];
            if (decl->in_type && sema_types_equals(type, decl->in_type) && slice_eq(&decl->name, name)) {
                if (decl->sema_type->type != SEMA_TYPE_FUNCTION) {
                    sema_err("in_type value is not a function");
                    return NULL;
                }
                return decl;
            }
        }
    }
    return NULL;
}

SemaScopeDecl *sema_module_resolve_scope_decl(SemaModule *sema, Slice *name) {
    for (ssize_t i = (ssize_t)vec_len(sema->scopes) - 1; i >= 0; i--) {
        SemaScope *scope = &sema->scopes[i];
        for (size_t j = 0; j < vec_len(scope->decls); j++) {
            SemaScopeDecl *decl = scope->decls[j];
            if (!decl->in_type && slice_eq(&decl->name, name)) {
                return decl;
            }
        }
    }
    return NULL;
}

SemaScopeDecl *sema_module_resolve_public_decl(SemaModule *sema, Slice *name) {
    for (size_t j = 0; j < vec_len(sema->public_decls); j++) {
        SemaScopeDecl *decl = sema->public_decls[j];
        if (!decl->in_type && slice_eq(&decl->name, name)) {
            return decl;
        }
    }
    return NULL;
}

void sema_module_push_defer(SemaModule *sema, AstDefer *defer) {
    SemaScope *scope = vec_top(sema->scopes);
    scope->defers = vec_push(scope->defers, &defer);
}

void sema_module_append_ext_funcs_from(SemaModule *sema, SemaModule *from) {
    for (size_t i = 0; i < vec_len(from->public_decls); i++) {
        SemaScopeDecl *decl = from->public_decls[i];
        if (decl->in_type) {
            sema_module_push_decl(sema, decl);
        }
    }
}

SemaScopeDecl *sema_module_push_decl(SemaModule *sema, SemaScopeDecl *decl) {
    if (sema_module_resolve_scope_decl(sema, &decl->name)) {
        sema_err("`{slice}` was already declared", &decl->name);
        return NULL;
    }
    SemaScope *scope = vec_top(sema->scopes);
    scope->decls = vec_push(scope->decls, &decl);
    return decl;
}

SemaScopeDecl *sema_module_push_public_decl(SemaModule *sema, SemaScopeDecl *decl) {
    if (!sema_module_push_decl(sema, decl)) {
        return NULL;
    }
    sema->public_decls = vec_push(sema->public_decls, &decl);
    return decl;
}

void sema_module_push_scope(SemaModule *sema) {
    SemaScope scope = {
        .decls = vec_new(SemaScopeDecl*),
        .defers = vec_new(AstDefer*) // TODO: defer inside while loop
    };
    sema->scopes = vec_push(sema->scopes, &scope);
}

void sema_module_pop_scope(SemaModule *sema) {
    vec_pop(sema->scopes);
}

void sema_module_push_primitives(SemaModule *sema) {
    #define PP(name) sema_module_push_decl(sema, sema_scope_decl_new_type(slice_from_cstr(#name), sema_type_primitive_##name()));
	PP(i8); PP(i16); PP(i32); PP(i64);
    PP(u8); PP(u16); PP(u32); PP(u64);
    PP(f32); PP(f64);
	PP(bool); PP(void);
}

AstDefer **sema_module_resolve_defers(SemaModule *sema) {
    AstDefer **result = vec_new(AstDefer*);
    for (ssize_t i = (size_t)vec_len(sema->scopes) - 1; i >= 0; i--) {
        SemaScope *scope = &sema->scopes[i];
        for (ssize_t j = (size_t)vec_len(scope->defers) - 1; j >= 0; j--) {
            result = vec_push(result, &scope->defers[j]);
        }
    }
    return result;
}

void sema_module_set_returns(SemaModule *sema, SemaType *returns) {
    sema->returning = returns;
}

SemaType *sema_module_returns(SemaModule *sema) {
    return sema->returning;
}

void sema_module_fail(SemaModule *sema) {
    sema->failed = true;
}
