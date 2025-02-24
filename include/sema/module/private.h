#pragma once

#include <stdbool.h>
#include "core/log.h"
#include "core/slice.h"
#include "ast/api/defer.h"
#include "sema/module/module.h"
#include "sema/type.h"
#include "sema/module/decls.h"
#include "sema/module.h"

#define sema_err(fmt, ...) \
	do { \
		hob_log(LOGE, fmt, ##__VA_ARGS__); \
		sema_module_fail(sema); \
	} while (0)

SemaScopeDecl *sema_module_resolve_scope_decl(SemaModule *sema, Slice *name);
SemaScopeDecl *sema_module_resolve_public_decl(SemaModule *sema, Slice *name);
void sema_module_push_defer(SemaModule *sema, AstDefer *defer);
void sema_module_append_ext_funcs_from(SemaModule *sema, SemaModule *from);
SemaScopeDecl *sema_module_push_decl(SemaModule *sema, SemaScopeDecl *decl);
SemaScopeDecl *sema_module_push_public_decl(SemaModule *sema, SemaScopeDecl *decl);
SemaScopeDecl *sema_module_resolve_ext_func(SemaModule *sema, Slice *name, SemaType *type);

void sema_module_push_primitives(SemaModule *sema);
void sema_module_push_scope(SemaModule *sema);
void sema_module_pop_scope(SemaModule *sema);
SemaScope *sema_module_top_scope(SemaModule *sema);

SemaScopeDecl *sema_scope_decl_new_type(Slice name, SemaType *sema_type);
SemaScopeDecl *sema_scope_decl_new_value(Slice name, SemaType *type, bool constant);
SemaScopeDecl *sema_scope_decl_new_in_type_value(Slice name, SemaType *in_type, SemaType *type, bool constant);
SemaScopeDecl *sema_scope_decl_new_module(Slice name, SemaModule *module);

AstDefer **sema_module_resolve_defers(SemaModule *sema);

void sema_module_fail(SemaModule *sema);
void sema_module_set_returns(SemaModule *sema, SemaType *returns);
SemaType *sema_module_returns(SemaModule *sema);

