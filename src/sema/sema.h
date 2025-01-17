#pragma once

#include <llvm-c/Core.h>
#include "lexer/lex.h"
#include "parser/parsers.h"
#include "core/slice.h"
#include "type.h"

typedef struct _SemaTypeDecl {
	SemaType *type;
	Slice name;
} SemaTypeDecl;

typedef struct _SemaValueDecl {
	SemaType *type;
	Slice name;
	union {
		LLVMValueRef llvm_value;
	};
} SemaValueDecl;

typedef struct {
	Slice name;
	struct _SemaModule *module;
} SemaModuleUsage;

typedef struct {
	SemaValueDecl **decls;
	SemaTypeDecl **types;
} SemaScope;

struct _SemaProject;

typedef struct _SemaModule {
	SemaScope *scopes;
	SemaModuleUsage *modules;
	SemaType *returning;
	bool failed;
	AstModule *ast;
	struct _SemaProject *project;
} SemaModule;

#define sema_err(fmt, ...) \
	do { \
		hob_log(LOGE, fmt, ##__VA_ARGS__); \
		sema->failed = true; \
	} while (0)

SemaModule *sema_new_module(AstModule *module);
void sema_module_read(SemaModule *sema);
void sema_module(SemaModule *sema);
void sema_push_scope(SemaModule *sema);
SemaScope *sema_pop_scope(SemaModule *sema);
SemaType *sema_resolve_ident_type(SemaModule *sema, Slice *name);
void sema_push_primitives(SemaModule *sema);
SemaType *sema_resolve_decl_type(SemaModule *sema, Slice *name);

void sema_push_type(SemaModule *sema, Slice name, SemaType *type);
SemaValueDecl *sema_push_decl(SemaModule *sema, Slice name, SemaType *type);
SemaValueDecl *sema_resolve_value_decl(SemaModule *sema, Slice *name);

typedef struct _SemaProject {
	SemaModule **modules;
} SemaProject;

SemaProject *sema_project_new();
SemaModule *sema_project_add_module(SemaProject *project, const char *path);
SemaModule *sema_project(SemaProject *project);
SemaModule *sema_resolve_module(SemaModule *sema, Slice *name);
