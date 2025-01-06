#pragma once

#include <stdint.h>
#include "ast_type.h"
#include "sema/type.h"
#include "core/fatptr.h"
#include "core/vec.h"

typedef enum {
	BINOP_ADD,
	BINOP_SUB,
	BINOP_MUL,
	BINOP_DIV,
	BINOP_EQ,
	BINOP_NEQ,
	BINOP_GT,
	BINOP_GE,
	BINOP_LT,
	BINOP_LE
} BinopType;

typedef enum {
	EXPR_IDENT,
	EXPR_INTEGER,
	EXPR_CHAR,
	EXPR_BOOL,
	EXPR_FUNCALL,
	EXPR_AS,
//	EXPR_STRING,
	EXPR_BINOP,
//	EXPR_UNARY
} ExprType;

struct _Expr;

typedef struct {
	BinopType type;
	struct _Expr *left, *right;
} ExprBinop;

typedef enum {
	UNARY_MINUS
} UnaryType;

typedef Vec FuncallArgs;

typedef struct {
	FatPtr name;
	FuncallArgs args;
} Funcall;

typedef struct {
	UnaryType type;
	struct _Expr *expr;
} ExprUnary;

typedef struct {
	AstType type;
	struct _Expr *expr;
} ExprAs;

typedef struct _Expr {
	ExprType type;
	Type sema_type;
	union {
		ExprBinop binop;
		Funcall funcall;
		ExprUnary unary;
		ExprAs as;
		FatPtr ident;
		FatPtr str;
		uint64_t integer;
		bool boolean;
	};
	struct _Expr *parent; // for parser
} Expr;

void expr_free(Expr *expr);
