#include "module.h"
#include "ast/interface/module.h"
#include "core/location.h"
#include "parser/interface/parser.h"
#include "sema/interface/scope.h"
#include "ast/module.h"
#include "sema/scope.h"
#include "sema/project.h"
#include "core/assert.h"

SemaScopeStack *sema_module_swap_ss(SemaModule *sema, SemaScopeStack *ss) {
    SemaScopeStack *result = sema->ss;
    sema->ss = ss;
    return result;
}

void sema_module_print_line_error_at(SemaModule *sema, FileLocation at) {
    parser_print_line_error_at(sema->ast->parser, at);
}

void sema_module_push_scope(SemaModule *sema, SemaScope *scope) {
    assert(sema->ss, "pushing scope outside of scope stack");
    sema_ss_push_scope(sema->ss, scope);
}

void sema_module_pop_scope(SemaModule *sema) {
    assert(sema->ss, "poping scope outside of scope stack");
    sema_ss_pop_scope(sema->ss);
}

void sema_module_push_global_decl(SemaModule *sema, FileLocation at, SemaDecl *decl, bool public) {
    sema_scope_push_decl(sema, at, sema->private, decl);
    if (public) {
        sema_scope_push_decl(sema, at, sema->public, decl);
    }
}

void sema_module_push_decl(SemaModule *sema, FileLocation at, SemaDecl *decl, bool public) {
    if (sema->ss) {
        if (public) {
            SEMA_ERROR(at, "trying to push public declaration in local scope");
        }
        sema_scope_push_decl(sema, at, sema_ss_top(sema->ss), decl);
    } else {
        sema_module_push_global_decl(sema, at, decl, public);
    }
}

SemaDecl *sema_module_resolve_decl(SemaModule *sema, Slice *name, SemaType *in_type) {
    if (sema->ss) {
        SemaDecl *trial = sema_ss_resolve_decl(sema, sema->ss, name, in_type);
        if (trial) {
            return trial;
        }
    }
    return sema_scope_resolve_decl(sema, sema->private, name, in_type);
}

SemaDecl *sema_module_resolve_req_decl(SemaModule *sema, FileLocation at, Slice *name, SemaType *in_type) {
    SemaDecl *decl = sema_module_resolve_decl(sema, name, in_type);
    if (!decl) {
        SEMA_ERROR(at, "`{slice}` is undeclared", name);
    }
    return decl;
}

SemaType *sema_module_returns(SemaModule *sema) {
    if (!sema->ss) {
        return NULL;
    }
    return sema_ss_returns(sema->ss);
}

bool sema_module_is_global(SemaModule *sema) {
    return sema->ss == NULL;
}

SemaArchInfo *sema_module_arch_info(SemaModule *sema) {
    return &sema->project->arch_info;
}

void sema_module_push_loop(SemaModule *sema, SemaLoop *loop) {
    return sema_ss_push_loop(sema->ss, loop);
}
void sema_module_pop_loop(SemaModule *sema) {
    return sema_ss_pop_loop(sema->ss);
}
SemaLoop *sema_module_resolve_loop(SemaModule *sema, Slice *name) {
    return name ? 
        sema_ss_resolve_named_loop(sema->ss, name) :
        sema_ss_resolve_loop(sema->ss);
}
