#pragma once

#include "ast/api/body.h"
#include "sema/module/scopes/scope.h"
#include "sema/module/decls.h"
#include "sema/type.h"
#include "sema/module.h"
#include "core/slice.h"

SemaDecl *sema_ss_resolve_decl(SemaScopeStack *stack, SemaType *type, Slice *name);
AstBody *sema_ss_current_body(SemaScopeStack *stack);
SemaDecl *sema_ss_push_decl(SemaScopeStack *stack, SemaDecl *decl);
void sema_ss_push_body_scope(SemaScopeStack *stack, AstBody *body);
void sema_ss_push_scope(SemaScopeStack *stack);
void sema_ss_pop_scope(SemaScopeStack *stack);
SemaType *sema_ss_returns(SemaScopeStack *stack);
