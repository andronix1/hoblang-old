#pragma once

#include "ast/api.h"
#include "core/slice.h"
#include "../path.h"

typedef struct {
    AstExpr *of;
    AstInnerPath path;
} AstExprGetInner;
