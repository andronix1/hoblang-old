#pragma once

#include "ast/api/expr.h"
#include "ast/api/body.h"

typedef struct {
	AstExpr *expr;
	AstBody *body;
} AstWhile;
