#pragma once

#include <malloc.h>
#include "ast/interface/expr.h"
#include "ast/node/stmt/asm.h"
#include "ast/node/stmt/assign.h"
#include "ast/node/stmt/defer.h"
#include "ast/node/stmt/loop_control.h"
#include "ast/node/stmt/return.h"
#include "ast/node/stmt/if.h"
#include "ast/node/stmt/while.h"

typedef enum {
    AST_NODE_STMT_IF,
	AST_NODE_STMT_RETURN,
	AST_NODE_STMT_WHILE,
	AST_NODE_STMT_INLINE_ASM,
	AST_NODE_STMT_DEFER,
	AST_NODE_STMT_EXPR,
	AST_NODE_STMT_BREAK,
	AST_NODE_STMT_CONTINUE,
	AST_NODE_STMT_ASSIGN
} AstKind;

typedef struct AstNodeStmt {
    AstKind kind;
    union {
        AstIf *if_else;
        AstReturn *ret;
        AstWhile *while_loop;
        AstInlineAsm *inline_asm;
        AstDefer *defer;
        AstExpr *expr;
        AstBreak *break_loop;
        AstContinue *continue_loop;
        AstAssign assign;
    };
} AstNodeStmt;

static inline AstNodeStmt *ast_node_stmt_new_inline_asm(AstInlineAsm *inline_asm) {
    AstNodeStmt *result = malloc(sizeof(AstNodeStmt));
    result->kind = AST_NODE_STMT_INLINE_ASM;
    result->inline_asm = inline_asm;
    return result;
}

static inline AstNodeStmt *ast_node_stmt_new_defer(AstDefer *defer) {
    AstNodeStmt *result = malloc(sizeof(AstNodeStmt));
    result->kind = AST_NODE_STMT_DEFER;
    result->defer = defer;
    return result;
}

static inline AstNodeStmt *ast_node_stmt_new_assign(AstAssign assign) {
    AstNodeStmt *result = malloc(sizeof(AstNodeStmt));
    result->kind = AST_NODE_STMT_ASSIGN;
    result->assign = assign;
    return result;
}

static inline AstNodeStmt *ast_node_stmt_new_expr(AstExpr *expr) {
    AstNodeStmt *result = malloc(sizeof(AstNodeStmt));
    result->kind = AST_NODE_STMT_EXPR;
    result->expr = expr;
    return result;
}

static inline AstNodeStmt *ast_node_stmt_new_continue(AstContinue *continue_loop) {
    AstNodeStmt *result = malloc(sizeof(AstNodeStmt));
    result->kind = AST_NODE_STMT_CONTINUE;
    result->continue_loop = continue_loop;
    return result;
}

static inline AstNodeStmt *ast_node_stmt_new_break(AstBreak *break_loop) {
    AstNodeStmt *result = malloc(sizeof(AstNodeStmt));
    result->kind = AST_NODE_STMT_BREAK;
    result->break_loop = break_loop;
    return result;
}

static inline AstNodeStmt *ast_node_stmt_new_while(AstWhile *while_loop) {
    AstNodeStmt *result = malloc(sizeof(AstNodeStmt));
    result->kind = AST_NODE_STMT_WHILE;
    result->while_loop = while_loop;
    return result;
}

static inline AstNodeStmt *ast_node_stmt_new_return(AstReturn *ret) {
    AstNodeStmt *result = malloc(sizeof(AstNodeStmt));
    result->kind = AST_NODE_STMT_RETURN;
    result->ret = ret;
    return result;
}

static inline AstNodeStmt *ast_node_stmt_new_if(AstIf *if_else) {
    AstNodeStmt *result = malloc(sizeof(AstNodeStmt));
    result->kind = AST_NODE_STMT_IF;
    result->if_else = if_else;
    return result;
}

