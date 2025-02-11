#pragma once

#include <llvm-c/Core.h>
#include "core/slice.h"
#include "../type/api.h"
#include "api.h"

#define sema_err(fmt, ...) \
	do { \
		hob_log(LOGE, fmt, ##__VA_ARGS__); \
		sema_module_fail(sema); \
	} while (0)

typedef enum {
    SEMA_SCOPE_DECL_TYPE,
    SEMA_SCOPE_DECL_MODULE,
    SEMA_SCOPE_DECL_VALUE,
} SemaScopeDeclType;

typedef struct SemaScopeValueDecl {
    SemaType *type;
    // TODO: abstract const value
    AstExpr *integer_expr;
    
    // sema
    bool constant;
    LLVMValueRef llvm_value; // TODO: i don't like it)
} SemaScopeValueDecl;

typedef struct SemaScopeDecl {
    SemaScopeDeclType type;
    Slice name;
    union {
        SemaType *sema_type;
        SemaScopeValueDecl value_decl;
        SemaModule *module;
    };
} SemaScopeDecl;


SemaScopeDecl *sema_module_resolve_scope_decl(SemaModule *sema, Slice *name);
SemaScopeDecl *sema_module_resolve_public_decl(SemaModule *sema, Slice *name);
void sema_module_push_defer(SemaModule *sema, AstDefer *defer);
SemaScopeDecl *sema_module_push_decl(SemaModule *sema, SemaScopeDecl *decl);
SemaScopeDecl *sema_module_push_public_decl(SemaModule *sema, SemaScopeDecl *decl);

void sema_module_push_primitives(SemaModule *sema);
void sema_module_push_scope(SemaModule *sema);
void sema_module_pop_scope(SemaModule *sema);

SemaScopeDecl *sema_scope_decl_new_type(Slice name, SemaType *sema_type);
SemaScopeDecl *sema_scope_decl_new_value(Slice name, SemaType *type, bool constant);
SemaScopeDecl *sema_scope_decl_new_module(Slice name, SemaModule *module);

AstDefer **sema_module_resolve_defers(SemaModule *sema);

void sema_module_fail(SemaModule *sema);
void sema_module_set_returns(SemaModule *sema, SemaType *returns);
SemaType *sema_module_returns(SemaModule *sema);
