#pragma once

#include "../value.h"

typedef struct {
	AstValue value;
	struct _AstExpr *args;
} AstFuncCall;
