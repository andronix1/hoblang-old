#pragma once

#include <stdbool.h>
#include "ast/api/defer.h"
#include "ast/api/module.h"
#include "ast/api/body.h"
#include "sema/arch/bits/bits.h"
#include "sema/module/private.h"
#include "sema/module/loop/loop.h"
#include "sema/project.h"

typedef struct SemaScope {
    SemaDecl **decls;
	AstBody *body;
} SemaScope;

typedef struct SemaModule {
    const char *path;
	AstModule *ast;
	bool failed;
    SemaArchInfo arch_info;

    // internal
    SemaProject *project; // TODO: API
    SemaDecl **public_decls;
	SemaScope *scopes;
	SemaLoop **loops;
	SemaType *returning;
} SemaModule;


