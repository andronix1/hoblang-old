#pragma once

#include "ast/interface/expr.h"

typedef enum {
    AST_EXPR_UNARY_ARITHMETIC_MINUS,
} AstExprUnaryArithmeticKind;

typedef enum {
    AST_EXPR_UNARY_BOOL_NOT,
} AstExprUnaryBoolKind;

typedef enum {
    AST_EXPR_UNARY_REF,
    AST_EXPR_UNARY_BOOL,
    AST_EXPR_UNARY_ARITHMETIC,
} AstExprUnaryKind;

typedef struct {
    AstExprUnaryKind kind;
    AstExpr *inner;
    union {
        AstExprUnaryBoolKind boolean;
        AstExprUnaryArithmeticKind arithmetic;
    };
} AstExprUnary;

static inline AstExprUnary ast_expr_unary_new_arithmetic(AstExpr *inner, AstExprUnaryArithmeticKind kind) {
    AstExprUnary result = {
        .kind = AST_EXPR_UNARY_ARITHMETIC,
        .inner = inner,
        .arithmetic = kind
    };
    return result;
}

static inline AstExprUnary ast_expr_unary_new_bool(AstExpr *inner, AstExprUnaryBoolKind kind) {
    AstExprUnary result = {
        .kind = AST_EXPR_UNARY_BOOL,
        .inner = inner,
        .boolean = kind
    };
    return result;
}

static inline AstExprUnary ast_expr_unary_new_ref(AstExpr *inner) {
    AstExprUnary result = {
        .kind = AST_EXPR_UNARY_REF,
        .inner = inner
    };
    return result;
}

