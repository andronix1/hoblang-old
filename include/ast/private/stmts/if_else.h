#pragma once

#include <stdbool.h>
#include "ast/api/expr.h"
#include "ast/api/body.h"

typedef struct {
	AstExpr *expr;
	AstBody *body;
} AstIfBody;

typedef struct {
	AstIfBody main;
	AstIfBody *else_ifs;
	bool has_else; // TODO: null
	AstBody *else_body;
} AstIfElse;
