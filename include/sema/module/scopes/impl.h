#pragma once

#include "sema/module/decls.h"
#include "ast/private/body.h"
#include "stack.h"

typedef struct SemaScope {
    SemaDecl **decls;
	AstBody *body;
} SemaScope;

