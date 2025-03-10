#pragma once

#include <stdbool.h>
#include "core/log.h"
#include "core/slice.h"
#include "ast/api/defer.h"
#include "ast/api/body.h"
#include "sema/module/scopes/scope.h"
#include "sema/module/module.h"
#include "sema/type.h"
#include "sema/module/decls.h"
#include "sema/value/value.h"
#include "sema/module.h"

#define SEMA_ERROR(at, fmt, ...) \
	do { \
		hob_log_at(LOGE, sema_module_path(sema), at, fmt, ##__VA_ARGS__); \
        sema_module_print_line_error_at(sema, at); \
		sema_module_fail(sema); \
	} while (0)
/*
#define sema_err(fmt, ...) \
	do { \
		hob_log(LOGE, fmt, ##__VA_ARGS__); \
		sema_module_fail(sema); \
	} while (0)
*/
SemaScopeStack *sema_module_ss_swap(SemaModule *sema, SemaScopeStack *ss);

SemaDecl *sema_module_resolve_scope_decl(SemaModule *sema, Slice *name);
SemaDecl *sema_module_resolve_public_decl(SemaModule *sema, Slice *name);

void sema_module_push_defer(SemaModule *sema, AstDefer *defer);
AstDefer **sema_module_defers_up_to(SemaModule *sema, AstBody *to);

void sema_module_append_all_from(SemaModule *sema, FileLocation at, bool public, SemaModule *from);
void sema_module_append_ext_funcs_from(SemaModule *sema, FileLocation at, SemaModule *from);
SemaDecl *sema_module_push_module_decl(SemaModule *sema, FileLocation at, bool public, SemaDecl *decl);
SemaDecl *sema_module_push_scope_decl(SemaModule *sema, FileLocation at, SemaDecl *decl);
SemaDecl *sema_module_resolve_ext_func(SemaModule *sema, Slice *name, SemaType *type);

void sema_module_push_primitives(SemaModule *sema);
void sema_module_push_body_scope(SemaModule *sema, AstBody *body);
void sema_module_push_scope(SemaModule *sema);
void sema_module_pop_scope(SemaModule *sema);
SemaScope *sema_module_top_scope(SemaModule *sema);

void sema_module_fail(SemaModule *sema);
SemaType *sema_module_returns(SemaModule *sema);

const char *sema_module_path(SemaModule *sema);

void sema_module_print_line_error_at(SemaModule *sema, FileLocation at);
