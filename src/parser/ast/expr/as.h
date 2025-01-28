#pragma once

#include "../expr.h"
#include "../type.h"

typedef struct {
	AstType type;
	struct _AstExpr *expr;
} AstExprAs;
