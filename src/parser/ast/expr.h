#pragma once

#include <stdint.h>
#include "expr/as.h"
#include "expr/binop.h"
#include "expr/func_call.h"
#include "expr/idx.h"

typedef enum {
	AST_EXPR_VALUE,
	AST_EXPR_INTEGER,
	AST_EXPR_CHAR,
	AST_EXPR_BOOL,
	AST_EXPR_FUNCALL,
	AST_EXPR_AS,
	AST_EXPR_BINOP,
	AST_EXPR_ARRAY,
	AST_EXPR_IDX,
} AstExprType;

struct _SemaType;

typedef struct _AstExpr {
	AstExprType type;
	union {
		AstExprBinop binop;
		AstFuncCall func_call;
		AstExprAs as;
		struct _AstExpr *array;
		AstIdx idx;
		Slice value;
		uint64_t integer;
		char character;
		bool boolean;
	};
	struct _SemaType *sema_type; // for sema
} AstExpr;

void print_ast_expr(FILE *stream, va_list *list);
