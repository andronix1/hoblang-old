#pragma once

typedef enum {
	AST_BINOP_AND,
	AST_BINOP_ADD,
	AST_BINOP_SUB,
	AST_BINOP_MUL,
	AST_BINOP_DIV,
	AST_BINOP_EQ,
	AST_BINOP_NEQ,
	AST_BINOP_GT,
	AST_BINOP_GE,
	AST_BINOP_LT,
	AST_BINOP_LE,
	AST_BINOP_OR,
	
	AST_BINOP_BITAND,
	AST_BINOP_BITOR,
	AST_BINOP_SHR,
	AST_BINOP_SHL,
	AST_BINOP_XOR,
} AstBinopType;

typedef struct {
	AstBinopType type;
	struct _AstExpr *left, *right;
} AstExprBinop;
