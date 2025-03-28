#pragma once

#include "ast/interface/expr.h"

typedef struct {
    AstExpr *callable;
    AstExpr **args;
} AstExprCall;

