#pragma once

#include "expr.h"

typedef struct {
	AstType type;
	struct _AstExpr *expr;
} AstExprAs;
