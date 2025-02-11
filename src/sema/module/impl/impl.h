#pragma once

#include "../private.h"
#include "ast/api.h"

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


