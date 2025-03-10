#include "ast/api/defer.h"
#include "ast/private/module.h"
#include "core/location.h"
#include "sema/arch/bits/private.h"
#include "sema/module/decls/decls.h"
#include "sema/module/impl.h"
#include "sema/module/decls/api.h"
#include "core/vec.h"
#include "sema/module/scopes/scope.h"
#include "sema/type/api.h"
#include "sema/type/private.h"
#include "sema/type/arch.h"
#include "sema/module/private.h"
#include "sema/module/decls/impl.h"
#include "ast/private/body.h"
#include "sema/value/api.h"
#include "sema/value/private.h"
#include "sema/module/scopes/private.h"
#include "parser/api.h"

#define ASSERT_SS() assert(sema->current_ss, "illegal operation outside of scope stack");

SemaScopeStack *sema_module_ss_swap(SemaModule *sema, SemaScopeStack *ss) {
    SemaScopeStack *result = sema->current_ss;
    sema->current_ss = ss;
    return result;
}

SemaDecl *sema_module_resolve_module_decl_in_type(SemaModule *sema, SemaType *type, Slice *name) {
    for (size_t j = 0; j < vec_len(sema->private_decls); j++) {
        SemaDecl *decl = sema->private_decls[j];
        if (slice_eq(&decl->name, name)) {
            if (type) {
                if (decl->in_type && sema_types_equals(decl->in_type, type)) { 
                    return decl;
                }
            } else if (!decl->in_type) {
                return decl;
            }
        }
    }
    return NULL;
}

SemaDecl *sema_module_resolve_module_ext_func(SemaModule *sema, Slice *name, SemaType *type) {
    SemaDecl *decl = sema_module_resolve_module_decl_in_type(sema, type, name);
    if (!decl) {
        return NULL;
    }
    if (sema_value_is_type_of(decl->value, SEMA_TYPE_FUNCTION)) {
        assert(0, "in_type value is not a function");
        return NULL;
    }
    return decl;
}

SemaDecl *sema_module_resolve_ext_func(SemaModule *sema, Slice *name, SemaType *type) {
    return sema_module_resolve_module_ext_func(sema, name, type);
}

SemaDecl *sema_module_resolve_scope_decl(SemaModule *sema, Slice *name) {
    if (sema->current_ss) {
        SemaDecl *in_ss = sema_ss_resolve_decl(sema->current_ss, NULL, name);
        if (in_ss) {
            return in_ss;
        }
    }
    return sema_module_resolve_module_decl_in_type(sema, NULL, name);
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
    if (!sema->current_ss) {
        return NULL;
    }
    return sema_ss_current_body(sema->current_ss);
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

void sema_module_append_all_from(SemaModule *sema, FileLocation at, bool public, SemaModule *from) {
    for (size_t i = 0; i < vec_len(from->public_decls); i++) {
        SemaDecl *decl = from->public_decls[i];
        if (!decl->in_type) {
            sema_module_push_module_decl(sema, at, public, decl);
        }
    }
}

void sema_module_append_ext_funcs_from(SemaModule *sema, FileLocation at, SemaModule *from) {
    for (size_t i = 0; i < vec_len(from->public_decls); i++) {
        SemaDecl *decl = from->public_decls[i];
        if (decl->in_type) {
            sema_module_push_module_decl(sema, at, false, decl);
        }
    }
}

SemaDecl *sema_module_push_scope_decl(SemaModule *sema, FileLocation at, SemaDecl *decl) {
    ASSERT_SS();
    return sema_ss_push_decl(sema->current_ss, decl);
}

SemaDecl *sema_module_push_module_decl(SemaModule *sema, FileLocation at, bool public, SemaDecl *decl) {
    if (sema_module_resolve_scope_decl(sema, &decl->name)) {
        SEMA_ERROR(at, "`{slice}` was already declared", &decl->name);
        return NULL;
    }
    sema->private_decls = vec_push(sema->private_decls, &decl);
    if (public) {
        sema->public_decls = vec_push(sema->public_decls, &decl);
    }
    return decl;
}

void sema_module_push_body_scope(SemaModule *sema, AstBody *body) {
    ASSERT_SS();
    sema_ss_push_body_scope(sema->current_ss, body);
}

void sema_module_push_scope(SemaModule *sema) {
    sema_module_push_body_scope(sema, NULL);
}

void sema_module_pop_scope(SemaModule *sema) {
    ASSERT_SS();
    sema_ss_pop_scope(sema->current_ss);
}

void sema_module_push_primitives(SemaModule *sema) {
    #define PD(name, type) sema_module_push_module_decl(sema, file_loc_new(), false, sema_decl_new(slice_from_cstr(#name), sema_value_type(type)))
    #define PP(name) sema_module_push_module_decl(sema, file_loc_new(), false, sema_decl_new(slice_from_cstr(#name), sema_value_type(sema_type_primitive_##name())))
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

const char *sema_module_path(SemaModule *sema) {
    return sema->ast->path;
}

SemaType *sema_module_returns(SemaModule *sema) {
    ASSERT_SS();
    return sema_ss_returns(sema->current_ss);
}

void sema_module_fail(SemaModule *sema) {
    sema->failed = true;
}

void sema_module_print_line_error_at(SemaModule *sema, FileLocation at) {
    parser_print_line_error_at(sema->ast->parser, at);
}
