#pragma once

#include "ast/interface/expr.h"

typedef struct {
    AstExpr *inner, *idx;
} AstExprIdx;

