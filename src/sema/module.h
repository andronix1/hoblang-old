#pragma once

#include <llvm-c/Core.h>
#include "type.h"

#define sema_err(fmt, ...) \
	do { \
		hob_log(LOGE, fmt, ##__VA_ARGS__); \
		sema->failed = true; \
	} while (0)

typedef enum {
    SEMA_SCOPE_DECL_TYPE,
    SEMA_SCOPE_DECL_MODULE,
    SEMA_SCOPE_DECL_VALUE,
} SemaScopeDeclType;

typedef struct _SemaScopeValueDecl {
    SemaType *type;
    LLVMValueRef llvm_value;
} SemaScopeValueDecl;

typedef struct _SemaScopeDecl {
    SemaScopeDeclType type;
    Slice name;
    union {
        SemaType *sema_type;
        SemaScopeValueDecl value_decl;
        struct _SemaModule *module;
    };
} SemaScopeDecl;

SemaScopeDecl *sema_scope_decl_new_type(Slice name, SemaType *sema_type);
SemaScopeDecl *sema_scope_decl_new_value(Slice name, SemaType *sema_type);
SemaScopeDecl *sema_scope_decl_new_module(Slice name, struct _SemaModule *module);

typedef struct {
    SemaScopeDecl **decls;
	AstDefer **defers;
} SemaScope;

typedef struct _SemaModule {
	AstModule *ast;
	struct _SemaProject *project;
	bool failed;

    // internal
    SemaScopeDecl **public_decls;
	SemaScope *scopes;
	SemaType *returning;
} SemaModule;

SemaModule *sema_module_new(struct _SemaProject *project, AstModule *module);

void sema_module_read(SemaModule *sema);
void sema_module(SemaModule *sema);
SemaScopeDecl *sema_module_resolve_scope_decl(SemaModule *sema, Slice *name);
SemaScopeDecl *sema_module_resolve_public_decl(SemaModule *sema, Slice *name);
void sema_module_push_defer(SemaModule *sema, AstDefer *defer);
SemaScopeDecl *sema_module_push_decl(SemaModule *sema, SemaScopeDecl *decl);
SemaScopeDecl *sema_module_push_public_decl(SemaModule *sema, SemaScopeDecl *decl);

void sema_module_push_primitives(SemaModule *sema);
void sema_module_push_scope(SemaModule *sema);
void sema_module_pop_scope(SemaModule *sema);

AstDefer **sema_module_resolve_defers(SemaModule *sema);