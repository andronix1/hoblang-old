#pragma once

#include "../type.h"
#include "../expr.h"

typedef struct {
	Slice name;
	AstType type;
	AstExpr expr;
	bool initializes, typed;
	
	struct _SemaScopeValueDecl *decl;
} AstVar;
