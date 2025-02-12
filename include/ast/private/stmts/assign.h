#pragma once

#include "ast/api/expr.h"

typedef struct {
	AstExpr *assign_expr;
	AstExpr *expr;
} AstAssign;
