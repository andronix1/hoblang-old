#pragma once

#include "ast/interface/expr.h"
#include "ast/interface/path.h"

typedef struct {
    AstExpr *inner;
    AstPath *path;
} AstExprInnerPath;
