#pragma once

#include "ast/api/expr.h"

typedef struct {
    AstExpr *of;
    AstExpr *idx;
} AstExprIdx;