#pragma once

#include "sema/module/ast.h"
#include "../type.h"
#include "../expr.h"

typedef struct {
	Slice name;
	AstType type;
	AstExpr *expr;
	bool initializes, typed;
	
	SemaScopeValueDecl *decl;
} AstVar;
