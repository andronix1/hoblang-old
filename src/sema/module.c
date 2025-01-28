#include "module.h"

SemaModule *sema_module_new(struct _SemaProject *project, AstModule *module) {
	SemaModule *result = malloc(sizeof(SemaModule));
	result->ast = module;
	result->project = project;
	result->public_decls = vec_new(SemaScopeDecl*);
	result->scopes = vec_new(SemaScope);
	result->failed = false;
	return result;
}

SemaScopeDecl *sema_scope_decl_new_type(Slice name, SemaType *sema_type) {
    SemaScopeDecl *result = malloc(sizeof(SemaScopeDecl));
    result->type = SEMA_SCOPE_DECL_TYPE;
    result->name = name;
    result->sema_type = sema_type;
    return result;
}

SemaScopeDecl *sema_scope_decl_new_value(Slice name, SemaType *type) {
    SemaScopeDecl *result = malloc(sizeof(SemaScopeDecl));
    result->type = SEMA_SCOPE_DECL_VALUE;
    result->name = name;
    result->value_decl.type = type;
    result->value_decl.llvm_value = NULL;
    return result;
}

SemaScopeDecl *sema_scope_decl_new_module(Slice name, SemaModule *module) {
    SemaScopeDecl *result = malloc(sizeof(SemaScopeDecl));
    result->type = SEMA_SCOPE_DECL_MODULE;
    result->name = name;
    result->module = module;
    return result;
}

SemaScopeDecl *sema_module_resolve_scope_decl(SemaModule *sema, Slice *name) {
    for (ssize_t i = (ssize_t)vec_len(sema->scopes) - 1; i >= 0; i--) {
        SemaScope *scope = &sema->scopes[i];
        for (size_t j = 0; j < vec_len(scope->decls); j++) {
            SemaScopeDecl *decl = scope->decls[j];
            if (slice_eq(&decl->name, name)) {
                return decl;
            }
        }
    }
    return NULL;
}

SemaScopeDecl *sema_module_resolve_public_decl(SemaModule *sema, Slice *name) {
    for (size_t j = 0; j < vec_len(sema->public_decls); j++) {
        SemaScopeDecl *decl = sema->public_decls[j];
        if (slice_eq(&decl->name, name)) {
            return decl;
        }
    }
    return NULL;
}

void sema_module_push_defer(SemaModule *sema, AstDefer *defer) {
    SemaScope *scope = vec_top(sema->scopes);
    scope->defers = vec_push(scope->defers, &defer);
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
    #define PP(name, type) sema_module_push_decl(sema, sema_scope_decl_new_type(slice_from_cstr(name), &primitives[type]));
	PP("i8", PRIMITIVE_I8);
    PP("i16", PRIMITIVE_I16);
    PP("i32", PRIMITIVE_I32);
    PP("i64", PRIMITIVE_I64);
    PP("u8", PRIMITIVE_U8);
    PP("u16", PRIMITIVE_U16);
    PP("u32", PRIMITIVE_U32);
    PP("u64", PRIMITIVE_U64);
	PP("bool", PRIMITIVE_BOOL);
	PP("void", PRIMITIVE_VOID);
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