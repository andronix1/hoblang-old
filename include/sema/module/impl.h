#pragma once

#include <stdbool.h>
#include "ast/api/defer.h"
#include "ast/api/module.h"
#include "ast/api/body.h"
#include "sema/module/private.h"
#include "sema/module/loop/loop.h"
#include "sema/project.h"

typedef struct SemaScope {
    SemaScopeDecl **decls;
	AstBody *body;
} SemaScope;

typedef struct SemaModule {
	AstModule *ast;
	bool failed;

    // internal
    SemaProject *project; // TODO: API
    SemaScopeDecl **public_decls;
	SemaScope *scopes;
	SemaLoop **loops;
	SemaType *returning;
} SemaModule;


