#pragma once

#include "core/location.h"
#include "ast/api/expr.h"

typedef struct {
    FileLocation loc;
	AstExpr *assign_expr;
	AstExpr *expr;
} AstAssign;
