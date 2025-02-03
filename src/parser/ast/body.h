#pragma once

#include <stdarg.h>
#include "stmts/var.h"
#include "stmts/if_else.h"
#include "stmts/defer.h"
#include "stmts/return.h"
#include "stmts/assign.h"
#include "stmts/while.h"
#include "stmts/asm.h"

typedef enum {
	AST_STMT_IF,
	AST_STMT_VAR,
	AST_STMT_RETURN,
	AST_STMT_WHILE,
	AST_STMT_INLINE_ASM,
	AST_STMT_DEFER,
	AST_STMT_EXPR,
	AST_STMT_ASSIGN
} AstStmtType;

typedef struct {
	AstStmtType type;
	union {
		AstVar var;
		AstExpr *expr;
		AstIfElse if_else;
		AstReturn ret;
		AstDefer defer;
		AstWhile while_loop;
		AstInlineAsm inline_asm;
		AstAssign assign;
	};
} AstStmt;

typedef struct _AstBody {
	AstStmt *stmts;

	// sema
	AstDefer **defers;
} AstBody;

void print_ast_body(FILE *stream, va_list *list);
