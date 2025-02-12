#pragma once

#include <stdbool.h>
#include "ast/api/expr.h"
#include "defer.h"

typedef struct {
    // TODO: NULL
	AstExpr *expr;
	bool has_value;
} AstReturn;
