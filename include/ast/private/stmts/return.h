#pragma once

#include <stdbool.h>
#include "ast/api/expr.h"
#include "defer.h"

typedef struct {
	AstExpr *expr;
	bool has_value;

	AstDefer **defers;
} AstReturn;
