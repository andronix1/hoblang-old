#pragma once

#include "expr.h"

typedef struct {
	Slice name;
	struct _AstExpr *args;
} AstFuncCall;
