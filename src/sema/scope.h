#pragma once

#include "core/location.h"
#include "core/slice/slice.h"
#include "sema/interface/decl.h"
#include "sema/interface/type.h"
#include "sema/interface/module.h"
#include "sema/interface/loop.h"

typedef struct SemaScope {
    SemaDecl **decls;
    SemaType *returns;
} SemaScope;

typedef struct {
    SemaScope **scopes;
    SemaLoop **loops;
} SemaScopeStack;

SemaScope *sema_scope_new(SemaType *returns);
SemaDecl *sema_scope_resolve_decl(SemaModule *sema, SemaScope *scope, Slice *name, SemaType *in_type);
void sema_scope_push_decl(SemaModule *sema, FileLocation at, SemaScope *scope, SemaDecl *decl);

SemaScopeStack sema_scope_stack_new();
void sema_ss_push_scope(SemaScopeStack *ss, SemaScope *scope);
void sema_ss_push_loop(SemaScopeStack *ss, SemaLoop *loop);
void sema_ss_pop_scope(SemaScopeStack *ss);
void sema_ss_pop_loop(SemaScopeStack *ss);
SemaScope *sema_ss_top(SemaScopeStack *ss);
SemaDecl *sema_ss_resolve_decl(SemaModule *sema, SemaScopeStack *ss, Slice *name, SemaType *in_type);
SemaType *sema_ss_returns(SemaScopeStack *ss);

void sema_ss_push_loop(SemaScopeStack *ss, SemaLoop *loop);
void sema_ss_pop_loop(SemaScopeStack *ss);
SemaLoop *sema_ss_resolve_loop(SemaScopeStack *ss);
SemaLoop *sema_ss_resolve_named_loop(SemaScopeStack *ss, Slice *name);
