#pragma once

#include "expr.h"
#include "../value.h"

typedef struct {
	AstValue value;
	struct _AstExpr *args;
} AstFuncCall;
