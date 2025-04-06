#pragma once

#include "ast/interface/expr.h"

typedef enum {
    AST_STMT_ASSIGN_DIRECT,
    AST_STMT_ASSIGN_BINOP
} AstAssignKind;

typedef struct {
    AstAssignKind kind;
    AstExpr *assign_to;
    AstExpr *expr;

    union {
        AstBinopKind binop;
    };
} AstAssign;

static inline AstAssign ast_assign_new_binop(AstExpr *to, AstExpr *value, AstBinopKind binop) {
    AstAssign result;
    result.kind = AST_STMT_ASSIGN_BINOP;
    result.assign_to = to;
    result.expr = value;
    result.binop = binop;
    return result;
}
static inline AstAssign ast_assign_new_direct(AstExpr *to, AstExpr *value) {
    AstAssign result;
    result.kind = AST_STMT_ASSIGN_DIRECT;
    result.assign_to = to;
    result.expr = value;
    return result;
}
