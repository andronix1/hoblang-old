#pragma once

#include "core/location.h"
#include "core/slice/slice.h"
#include "sema/interface/decl.h"
#include "sema/interface/type.h"
#include "sema/interface/module.h"

typedef struct SemaScope {
    SemaDecl **decls;
    SemaType *returns;
} SemaScope;

typedef struct {
    SemaScope **scopes;
} SemaScopeStack;

SemaScope *sema_scope_new(SemaType *returns);
SemaDecl *sema_scope_resolve_decl(SemaModule *sema, SemaScope *scope, Slice *name, SemaType *in_type);
void sema_scope_push_decl(SemaModule *sema, FileLocation at, SemaScope *scope, SemaDecl *decl);

SemaScopeStack sema_scope_stack_new();
void sema_ss_push_scope(SemaScopeStack *ss, SemaScope *scope);
void sema_ss_pop_scope(SemaScopeStack *ss);
SemaScope *sema_ss_top(SemaScopeStack *ss);
SemaDecl *sema_ss_resolve_decl(SemaModule *sema, SemaScopeStack *ss, Slice *name, SemaType *in_type);
SemaType *sema_ss_returns(SemaScopeStack *ss);
