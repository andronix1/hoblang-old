#pragma once

#include "../expr.h"
#include "body.h"

typedef struct {
	AstExpr *expr;
	struct _AstBody *body;
} AstIfBody;

typedef struct {
	AstIfBody main;
	AstIfBody *else_ifs;
	bool has_else;
	struct _AstBody *else_body;
} AstIfElse;
