#pragma once

#include <stdbool.h>
#include "ast/api/defer.h"
#include "ast/api/module.h"
#include "sema/module/private.h"
#include "sema/project.h"

typedef struct SemaScope {
    SemaScopeDecl **decls;
	AstDefer **defers;
} SemaScope;

typedef struct SemaModule {
	AstModule *ast;
	bool failed;

    // internal
    SemaProject *project; // TODO: API
    SemaScopeDecl **public_decls;
	SemaScope *scopes;
	SemaType *returning;
} SemaModule;


