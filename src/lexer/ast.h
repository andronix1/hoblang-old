#pragma once

#include "core/fatptr.h"
#include "expr/expr.h"

typedef struct {
	Token *name;
	Token *type;
	Expr *assign;
} AstLet;

typedef Expr AstFuncCallArg;
typedef Vec AstFuncCallArgs;

typedef struct {
	Token *name;
	AstFuncCallArgs args;
} AstFuncCall;

typedef enum {
	AST_STAT_LET,
} AstStatType;

typedef struct {
	AstStatType type;
	union {
		AstLet let;
		AstFuncCall func_call;
	};
} AstStat;

typedef Vec AstStats;

typedef struct {
	AstStats stats;
} AstCodeBlock;

typedef struct {
	Token *type;
	Token *name;
} AstFuncArg;

typedef Vec AstFuncArgs;

typedef struct {
	Token *name;
	Token *return_type;
	AstFuncArgs args;
} AstFuncDecl;

typedef enum {
	AST_NODE_FUNC,
	AST_NODE_LET,
} AstNodeType;

typedef struct {
	AstNodeType type;
	union {
		AstFuncDecl func;		
		AstLet let;		
	};
} AstNode;
