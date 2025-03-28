#pragma once

#include "ast/interface/expr.h"
#include "ast/interface/type.h"
#include "core/location.h"
#include "sema/nodes/shared/expr/as_kind.h"

typedef enum {
    AST_EXPR_AS_AUTO,
    AST_EXPR_AS_TYPE,
} AstExprAsKind;

typedef struct {
    AstExprAsKind kind;
    AstExpr *inner;
    FileLocation as_loc;
    union {
        AstType *type;
        FileLocation as_auto_loc;
    };
    struct {
        SemaAsKind kind;
    } sema;
} AstExprAs;
