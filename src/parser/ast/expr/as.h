#pragma once

#include "../type.h"

typedef struct {
	AstType type;
	struct _AstExpr *expr;
} AstExprAs;
