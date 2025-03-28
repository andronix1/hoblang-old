#pragma once

#include "ast/interface/expr.h"
#include "core/location.h"

typedef struct {
    AstExpr *left, *right;
    FileLocation binop_loc;
    AstBinopKind kind;
} AstExprBinop;
