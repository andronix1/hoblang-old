#pragma once

#include "ast/api/defer.h"
#include "../private.h"

typedef struct SemaScope {
    SemaScopeDecl **decls;
	AstDefer **defers;
} SemaScope;

typedef struct SemaModule {
	AstModule *ast;
	bool failed;

    // internal
    struct SemaProject *project; // TODO: API
    SemaScopeDecl **public_decls;
	SemaScope *scopes;
	SemaType *returning;
} SemaModule;


