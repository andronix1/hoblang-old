#pragma once

#include "core/fatptr.h"
#include "core/vec.h"
#include "expr.h"

typedef enum {
	AST_TYPE_IDENT
} AstTypeKind;

typedef struct {
	AstTypeKind type;
	union {
		FatPtr ident;
	};
} AstType;

typedef struct {
	FatPtr name;
	AstType type;
	Expr expr;
	bool initializes, typed;
} AstVar;

typedef struct {
	Expr expr;
	bool has_value;
} AstReturn;

typedef Vec AstStmts;

typedef struct {
	AstStmts stmts;
} AstBody;

typedef struct {
	Expr expr;
	AstBody body;
} AstIfBody;

typedef Vec AstIfBodies;

typedef struct {
	AstIfBody main;
	AstIfBodies else_ifs;
	bool has_else;
	AstBody else_body;
} AstIf;

typedef enum {
	AST_STMT_IF,
	AST_STMT_VAR,
	AST_STMT_FUNCALL,
	AST_STMT_RETURN
} AstStmtType;

typedef struct {
	AstStmtType type;
	union {
		AstVar var;
		AstIf if_else;
		Funcall funcall; // yup, kinda funny)))
		AstReturn ret;
	};
} AstStmt;

typedef struct {
	AstType type;
	FatPtr name;
} AstFuncArg;

typedef Vec AstFuncArgs;

typedef struct {
	FatPtr name;
	AstType returning;
	AstFuncArgs args;
	AstBody body;
} AstFuncDecl;

typedef enum {
	AST_MODULE_NODE_FUNC,
	AST_MODULE_NODE_VAR,
} AstModuleNodeType;

typedef struct {
	AstModuleNodeType type;
	union {
		AstFuncDecl func_decl;		
		AstVar var;		
	};
} AstModuleNode;

typedef Vec AstModuleNodes;

typedef struct {
	AstModuleNodes nodes;
} AstModule;

void ast_print_module(AstModule *module);
