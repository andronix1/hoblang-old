#pragma once

#include "body.h"

typedef struct {
	AstExpr *expr;
	struct _AstBody *body;
} AstWhile;
