#pragma once

#include <stdbool.h>
#include "sema/module/decls.h"
#include "../type.h"
#include "ast/api/expr.h"

typedef struct {
	Slice name;
	AstType type;
	AstExpr *expr;
	bool initializes, typed;
	
	SemaScopeValueDecl *decl;
} AstVar;
