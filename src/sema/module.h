#pragma once

#include <stdbool.h>
#include "ast/interface/module.h"
#include "core/location.h"
#include "sema/arch/info.h"
#include "sema/interface/scope.h"
#include "sema/interface/scope.h"
#include "sema/interface/decl.h"
#include "sema/interface/type.h"
#include "sema/interface/project.h"
#include "core/log.h"
#include "sema/scope.h"

typedef struct SemaModule {
    const char *path;
	AstModule *ast;
	bool failed;

    SemaScope *public;
    SemaScope *private;
    SemaScopeStack *ss;

    SemaProject *project;
} SemaModule;

void sema_module_push_scope(SemaModule *sema, SemaScope *scope);
void sema_module_pop_scope(SemaModule *sema);
void sema_module_push_decl(SemaModule *sema, FileLocation at, SemaDecl *decl, bool public);
SemaDecl *sema_module_resolve_decl(SemaModule *sema, Slice *name, SemaType *in_type);
SemaDecl *sema_module_resolve_req_decl(SemaModule *sema, FileLocation at, Slice *name, SemaType *in_type);
void sema_module_print_line_error_at(SemaModule *sema, FileLocation at);
SemaType *sema_module_returns(SemaModule *sema);
SemaScopeStack *sema_module_swap_ss(SemaModule *sema, SemaScopeStack *ss);
bool sema_module_is_global(SemaModule *sema);
SemaArchInfo *sema_module_arch_info(SemaModule *sema);

#define SEMA_ERROR(at, fmt, ...) \
	do { \
		hob_log_at(LOGE, sema->path, at, fmt, ##__VA_ARGS__); \
        sema_module_print_line_error_at(sema, at); \
		sema->failed = true; \
	} while (0)
