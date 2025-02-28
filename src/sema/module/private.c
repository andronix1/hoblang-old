#include "ast/api/defer.h"
#include "ast/private/module.h"
#include "core/location.h"
#include "parser/private.h"
#include "sema/arch/bits/private.h"
#include "sema/module/impl.h"
#include "sema/module/decls/api.h"
#include "core/vec.h"
#include "sema/type/private.h"
#include "sema/type/arch.h"
#include "sema/module/private.h"
#include "sema/module/decls/impl.h"
#include "sema/type/api.h"
#include "ast/private/body.h"
#include "sema/value/api.h"
#include "sema/value/private.h"

SemaDecl *sema_module_resolve_ext_func(SemaModule *sema, Slice *name, SemaType *type) {
    for (ssize_t i = (ssize_t)vec_len(sema->scopes) - 1; i >= 0; i--) {
        SemaScope *scope = &sema->scopes[i];
        for (size_t j = 0; j < vec_len(scope->decls); j++) {
            SemaDecl *decl = scope->decls[j];
            if (decl->in_type && slice_eq(&decl->name, name) && sema_types_equals(type, decl->in_type)) {
                if (sema_value_is_type_of(decl->value, SEMA_TYPE_FUNCTION)) {
                    assert(0, "in_type value is not a function");
                    return NULL;
                }
                return decl;
            }
        }
    }
    return NULL;
}

SemaDecl *sema_module_resolve_scope_decl_in_type(SemaModule *sema, SemaType *type, Slice *name) {
    for (ssize_t i = (ssize_t)vec_len(sema->scopes) - 1; i >= 0; i--) {
        SemaScope *scope = &sema->scopes[i];
        for (size_t j = 0; j < vec_len(scope->decls); j++) {
            SemaDecl *decl = scope->decls[j];
            if (decl->in_type == type && slice_eq(&decl->name, name)) {
                return decl;
            }
        }
    }
    return NULL;
}

SemaDecl *sema_module_resolve_scope_decl(SemaModule *sema, Slice *name) {
    return sema_module_resolve_scope_decl_in_type(sema, NULL, name);
}

SemaDecl *sema_module_resolve_public_decl(SemaModule *sema, Slice *name) {
    for (size_t j = 0; j < vec_len(sema->public_decls); j++) {
        SemaDecl *decl = sema->public_decls[j];
        if (!decl->in_type && slice_eq(&decl->name, name)) {
            return decl;
        }
    }
    return NULL;
}

AstBody *sema_module_current_body(SemaModule *sema) {
    if (vec_len(sema->scopes) == 0) {
        return NULL;
    }
    return sema_module_top_scope(sema)->body;
}

void sema_module_push_defer(SemaModule *sema, AstDefer *defer) {
    AstBody* body = sema_module_current_body(sema);
    assert(body, "trying to push defer outside of body");
    body->defers = vec_push(body->defers, &defer);
}

AstDefer **sema_module_defers_up_to(SemaModule *sema, AstBody *to) {
    AstDefer **result = vec_new(AstDefer*);
    AstBody *body = sema_module_current_body(sema);
    assert(body, "trying to resolve defers outside of body");
    while (body) {
        for (ssize_t i = vec_len(body->defers) - 1; i >= 0; i--) {
            result = vec_push(result, &body->defers[i]);
        }
        if (body == to) {
            break;
        }
        body = body->parent;
    }
    return result;
}

void sema_module_append_ext_funcs_from(SemaModule *sema, FileLocation at, SemaModule *from) {
    for (size_t i = 0; i < vec_len(from->public_decls); i++) {
        SemaDecl *decl = from->public_decls[i];
        if (decl->in_type) {
            sema_module_push_decl(sema, at, decl);
        }
    }
}

SemaDecl *sema_module_push_decl(SemaModule *sema, FileLocation at, SemaDecl *decl) {
    if (sema_module_resolve_scope_decl(sema, &decl->name)) {
        SEMA_ERROR(at, "`{slice}` was already declared", &decl->name);
        return NULL;
    }
    SemaScope *scope = vec_top(sema->scopes);
    scope->decls = vec_push(scope->decls, &decl);
    return decl;
}

SemaDecl *sema_module_push_public_decl(SemaModule *sema, FileLocation at, SemaDecl *decl) {
    if (!sema_module_push_decl(sema, at, decl)) {
        return NULL;
    }
    sema->public_decls = vec_push(sema->public_decls, &decl);
    return decl;
}

SemaScope *sema_module_top_scope(SemaModule *sema) {
    return vec_top(sema->scopes);
}

void sema_module_push_body_scope(SemaModule *sema, AstBody *body) {
    AstBody *current_body = sema_module_current_body(sema);
    if (!body) {
        body = current_body;
    } else {
        body->parent = current_body;
    }
    SemaScope scope = {
        .decls = vec_new(SemaDecl*),
        .body = body
    };
    sema->scopes = vec_push(sema->scopes, &scope);
}

void sema_module_push_scope(SemaModule *sema) {
    return sema_module_push_body_scope(sema, NULL);
}

void sema_module_pop_scope(SemaModule *sema) {
    vec_pop(sema->scopes);
}

void sema_module_push_primitives(SemaModule *sema) {
    #define PD(name, type) sema_module_push_decl(sema, file_loc_new(), sema_decl_new(slice_from_cstr(#name), sema_value_type(type)))
    #define PP(name) sema_module_push_decl(sema, file_loc_new(), sema_decl_new(slice_from_cstr(#name), sema_value_type(sema_type_primitive_##name())))
    #define ADIPP(name, size) do { \
        if (sema->arch_info.ints & SEMA_INT_##size) { \
            PP(name); \
        } \
    } while (0)
    #define ADFPP(name, size) do { \
        if (sema->arch_info.floats & SEMA_FLOAT_##size) { \
            PP(name); \
        } \
    } while (0)
    PP(u8);
	ADIPP(i8, 8);
    ADIPP(i16, 16); ADIPP(u16, 16);
    ADIPP(i32, 32); ADIPP(u32, 32);
    ADIPP(i64, 64); ADIPP(u64, 64);
    ADFPP(f32, 32); ADFPP(f64, 64);
	PP(bool); PP(void);
    PD(usize, sema_arch_usize(sema));
}

void sema_module_set_returns(SemaModule *sema, SemaType *returns) {
    sema->returning = returns;
}

const char *sema_module_path(SemaModule *sema) {
    return sema->ast->path;
}

SemaType *sema_module_returns(SemaModule *sema) {
    return sema->returning;
}

void sema_module_fail(SemaModule *sema) {
    sema->failed = true;
}

void sema_module_print_line_error_at(SemaModule *sema, FileLocation at) {
    parser_print_line_error_at(sema->ast->parser, at);
}
