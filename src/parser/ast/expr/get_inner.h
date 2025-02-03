#pragma once

#include "core/slice.h"
#include "../path.h"

typedef struct {
    struct _AstExpr *of;
    AstInnerPath path;
} AstExprGetInner;