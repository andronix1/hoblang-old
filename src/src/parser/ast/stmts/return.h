#pragma once

#include "defer.h"

typedef struct {
	AstExpr expr;
	bool has_value;
} AstReturn;
