#pragma once

#include <stdbool.h>
#include "ast/api/expr.h"
#include "core/location.h"
#include "defer.h"

typedef struct {
    FileLocation loc;

	AstExpr *expr;
	bool has_value;

	AstDefer **defers;
} AstReturn;
