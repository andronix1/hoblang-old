#pragma once

#include "ast/interface/expr.h"
#include "sema/nodes/shared/expr/idx_kind.h"

typedef struct {
    AstExpr *inner, *idx;

    struct {
        SemaIdxKind kind;
    } sema;
} AstExprIdx;

