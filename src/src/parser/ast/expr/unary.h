typedef enum {
    AST_UNARY_MINUS,
	AST_UNARY_BITNOT,
} AstUnaryType;

typedef struct {
    AstUnaryType type;
    struct _AstExpr *expr;
} AstExprUnary;