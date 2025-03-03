#pragma once

#include "ast/api/expr.h"
#include "core/slice.h"
#include "../path.h"

typedef struct {
    AstExpr *of;
    AstPath path;
} AstExprGetInner;
