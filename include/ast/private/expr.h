#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "path.h"
#include "expr/as.h"
#include "expr/binop.h"
#include "expr/unary.h"
#include "expr/call.h"
#include "expr/get_local.h"
#include "expr/get_inner.h"
#include "expr/idx.h"

/*
7 * a.b.c(1, 2).a
MUL(
	7,
	GET_INNER_PATH(
		CALL(
			GET_LOCAL_PATH(a.b.c),
			ARGS: [INT(1), INT(2)]
		),
		PATH(a)
	)
)
*/

typedef enum {
	AST_EXPR_GET_LOCAL_PATH,
	AST_EXPR_GET_INNER_PATH,
	AST_EXPR_IDX,
	AST_EXPR_INTEGER,
	AST_EXPR_FLOAT,
	AST_EXPR_CHAR,
	AST_EXPR_STR,
	AST_EXPR_BOOL,
	AST_EXPR_CALL,
	AST_EXPR_NOT,
	AST_EXPR_AS,
	AST_EXPR_BINOP,
	AST_EXPR_UNARY,
	AST_EXPR_ARRAY,
	AST_EXPR_REF,
} AstExprType;

typedef struct AstExpr AstExpr;

typedef struct AstExpr {
	AstExprType type;
	union {
		AstExprBinop binop;
		AstCall call;
		AstExprUnary unary;
		AstExprAs as;
		AstExprIdx idx;
		AstExpr **array;
		AstExpr *ref_expr;
		AstExpr *not_expr;
		AstExprGetLocal get_local;
		AstExprGetInner get_inner;
		Slice str;
		uint64_t integer;
		long double float_value;
		char character;
		bool boolean;
	};
	// for sema
	SemaValue *value;
} AstExpr;

AstExpr *ast_expr_get_local_path(AstPath path);
AstExpr *ast_expr_get_inner_path(AstExpr *of, AstInnerPath path);
AstExpr *ast_expr_idx(AstExpr *of, AstExpr *idx);
AstExpr *ast_expr_integer(uint64_t value);
AstExpr *ast_expr_float(long double value);
AstExpr *ast_expr_char(char value);
AstExpr *ast_expr_str(Slice value);
AstExpr *ast_expr_bool(bool value);
AstExpr *ast_expr_call(AstExpr *callable, AstExpr **args);
AstExpr *ast_expr_not(AstExpr *expr);
AstExpr *ast_expr_as(AstExpr *expr, AstType type);
AstExpr *ast_expr_binop(AstBinopType type, AstExpr *left, AstExpr *right);
AstExpr *ast_expr_unary(AstUnaryType type, AstExpr *expr);
AstExpr *ast_expr_array(AstExpr **values);
AstExpr *ast_expr_ref(AstExpr *expr);
