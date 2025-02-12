#pragma once

#include "ast/api/expr.h"

typedef struct AstCall {
	AstExpr *callable;
	AstExpr **args;
} AstCall;
