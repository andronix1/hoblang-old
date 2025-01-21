#pragma once

#include <stdarg.h>
#include "stmts/var.h"
#include "stmts/if_else.h"
#include "stmts/defer.h"
#include "stmts/return.h"
#include "stmts/assign.h"
#include "stmts/while.h"

typedef enum {
	AST_STMT_IF,
	AST_STMT_VAR,
	AST_STMT_FUNC_CALL,
	AST_STMT_RETURN,
	AST_STMT_WHILE,
	AST_STMT_DEFER,
	AST_STMT_ASSIGN
} AstStmtType;

typedef struct {
	AstStmtType type;
	union {
		AstVar var;
		AstIfElse if_else;
		AstFuncCall func_call;
		AstReturn ret;
		AstDefer defer;
		AstWhile while_loop;
		AstAssign assign;
	};
} AstStmt;

typedef struct _AstBody {
	AstStmt *stmts;

	// sema
	AstDefer **defers;
} AstBody;

void print_ast_body(FILE *stream, va_list *list);
