#pragma once

#include "sema/module/decls.h"
#include "ast/api/expr.h"
#include "ast/private/type.h"
#include "core/slice.h"

typedef struct {
	AstType type;
	Slice name;
	AstExpr *expr;

	SemaDecl *decl;
} AstConst;

