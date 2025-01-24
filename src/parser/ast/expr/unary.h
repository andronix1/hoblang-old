typedef enum {
    AST_UNARY_MINUS,
} AstUnaryType;

typedef struct {
    AstUnaryType type;
    struct _AstExpr *expr;
} AstExprUnary;