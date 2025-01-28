#pragma once

#include <stdint.h>
#include "expr/as.h"
#include "expr/binop.h"
#include "expr/unary.h"
#include "expr/func_call.h"

typedef enum {
	AST_EXPR_VALUE,
	AST_EXPR_INTEGER,
	AST_EXPR_CHAR,
	AST_EXPR_STR,
	AST_EXPR_BOOL,
	AST_EXPR_FUNCALL,
	AST_EXPR_NOT,
	AST_EXPR_AS,
	AST_EXPR_BINOP,
	AST_EXPR_UNARY,
	AST_EXPR_ARRAY,
	AST_EXPR_REF,
} AstExprType;

struct _SemaType;

typedef struct _AstExpr {
	AstExprType type;
	union {
		AstExprBinop binop;
		AstFuncCall func_call;
		AstExprUnary unary;
		AstExprAs as;
		struct _AstExpr *array;
		struct _AstExpr *not_expr;
		AstValue value;
		Slice str;
		uint64_t integer;
		char character;
		bool boolean;
	};
	struct _SemaType *sema_type; // for sema
} AstExpr;
