#pragma once

#include "core/fatptr.h"
#include "expr/expr.h"

typedef struct {
	Token *name;
} AstType;

typedef struct {
	Token *name;
	AstType type;
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
} AstStmtType;

typedef struct {
	AstStmtType type;
	union {
		AstLet let;
		AstFuncCall func_call;
	};
} AstStmt;

typedef Vec AstStmts;

typedef struct {
	AstStmts stmts;
} AstCodeBlock;

typedef struct {
	AstType type;
	Token *name;
} AstFuncArg;

typedef Vec AstFuncArgs;

typedef struct {
	Token *name;
	bool returning;
	AstType return_type;
	AstFuncArgs args;
} AstFuncDecl;

typedef enum {
	AST_MODULE_NODE_FUNC,
	AST_MODULE_NODE_LET,
} AstModuleNodeType;

typedef struct {
	AstModuleNodeType type;
	union {
		AstFuncDecl func;		
		AstLet let;		
	};
} AstModuleNode;
