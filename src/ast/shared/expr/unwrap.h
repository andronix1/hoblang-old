#pragma once

#include "ast/interface/expr.h"
#include "core/location.h"
#include "core/slice/slice.h"

typedef struct {
    AstExpr *inner;
    Slice output_name;
    FileLocation output_name_loc;
} AstExprUnwrap;
