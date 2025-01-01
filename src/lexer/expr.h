#pragma once

#include "tokenizer/tokens.h"
#include "core/vec.h"
#include "expr/expr.h"
#include "base.h"

typedef Vec RawExpr;

typedef enum {
	EXPR_PART_OPERATION,
	EXPR_PART_SCOPE,
	EXPR_PART_READY
} ExprPartType;

typedef enum {
	EXPR_PART_OP_ADD,
	EXPR_PART_OP_MINUS,
	EXPR_PART_OP_MULTIPLY,
	EXPR_PART_OP_DIVIDE,
} ExprPartOpType;

typedef struct {
	ExprPartType type;
	union {
		ExprPartOpType op_type;
		RawExpr scope;
		Expr ready;
	};
} ExprPart;

bool expr_raw_parse(Lexer *lexer, RawExpr *output, TokensSlice *tokens);
bool expr_parse(Lexer *lexer, Expr *output, TokensSlice *tokens);
