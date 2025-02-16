#pragma once

#include "ast/api/expr.h"

typedef enum {
    SEMA_EXPR_IDX_SLICE,
    SEMA_EXPR_IDX_ARRAY,
} SemaExprIdx;

typedef struct {
    AstExpr *of;
    AstExpr *idx;

    SemaExprIdx sema;
} AstExprIdx;