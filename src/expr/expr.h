#pragma once

#include "core/fatptr.h"

typedef enum {
	BINOP_ADD,
	BINOP_SUB,
	BINOP_MUL,
	BINOP_DIV,
	BINOP_EQ,
	BUNOP_NEQ,
	BINOP_GT,
	BINOP_GE,
	BINOP_LT,
	BINOP_LE
} BinopType;

typedef enum {
	EXPR_IDENT,
	EXPR_INTEGER,
	EXPR_BOOL,
	EXPR_STRING,
	EXPR_BINOP
} ExprType;

struct _Expr;

typedef struct {
	BinopType type;
	struct _Expr *left, *right;
} Binop;

typedef struct _Expr {
	BinopType type;
	union {
		Binop binop;
		FatPtr ident;
		FatPtr str;
		bool boolean;
	};
} Expr;
