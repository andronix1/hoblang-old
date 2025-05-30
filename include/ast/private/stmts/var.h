#pragma once

#include <stdbool.h>
#include "core/location.h"
#include "sema/module/decls.h"
#include "../type.h"
#include "ast/api/expr.h"
#include "sema/module/decls/decls.h"

typedef struct {
    FileLocation loc;

	Slice name;
	AstType type;
	AstExpr *expr;
	bool initializes, typed;
	
	SemaDecl *decl;
} AstVar;
