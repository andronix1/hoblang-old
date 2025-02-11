#pragma once

#include "ast/api.h"

typedef struct {
	AstExpr *callable;
	AstExpr **args;
} AstCall;
