#pragma once

#include <stdarg.h>
#include "ast/private/stmts/loop_control.h"
#include "stmts/var.h"
#include "stmts/if_else.h"
#include "stmts/defer.h"
#include "stmts/return.h"
#include "stmts/assign.h"
#include "stmts/while.h"
#include "stmts/asm.h"
#include "stmts/loop_control.h"
#include "stmts/const.h"

typedef enum {
	AST_STMT_IF,
	AST_STMT_CONST,
	AST_STMT_VAR,
	AST_STMT_RETURN,
	AST_STMT_WHILE,
	AST_STMT_INLINE_ASM,
	AST_STMT_DEFER,
	AST_STMT_EXPR,
	AST_STMT_BREAK,
	AST_STMT_CONTINUE,
	AST_STMT_ASSIGN
} AstStmtType;

typedef struct {
	AstStmtType type;
	union {
		AstVar var;
		AstConst constant;
		AstExpr *expr;
		AstIfElse if_else;
		AstReturn ret;
		AstDefer defer;
		AstWhile while_loop;
		AstInlineAsm inline_asm;
		AstAssign assign;
        AstStmtLoopControl break_loop;
        AstStmtLoopControl continue_loop;
	};
} AstStmt;

typedef struct AstBody {
	AstStmt *stmts;

	// sema
    AstBody *parent;
	AstDefer **defers;
} AstBody;
