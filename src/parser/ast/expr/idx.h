#pragma once

#include "expr.h"

typedef struct {
	struct _AstExpr *expr;
	struct _AstExpr *idx;
} AstIdx;
