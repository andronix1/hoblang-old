#include "ast/api/expr.h"

typedef enum {
    AST_UNARY_MINUS,
	AST_UNARY_BITNOT,
} AstUnaryType;

typedef struct {
    AstUnaryType type;
    AstExpr *expr;
} AstExprUnary;
