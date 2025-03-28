#pragma once

#include <malloc.h>
#include "./node/decl.h"
#include "./node/stmt.h"
#include "core/location.h"

typedef enum {
    AST_NODE_DECL,
    AST_NODE_STMT,
} AstNodeKind;

typedef struct AstNode {
    AstNodeKind kind;
    FileLocation loc;
    union {
        AstNodeDecl *decl;
        AstNodeStmt *stmt;
    };
} AstNode;

static inline AstNode *ast_node_new_stmt(FileLocation loc, AstNodeStmt *stmt) {
    AstNode *result = malloc(sizeof(AstNode));
    result->kind = AST_NODE_STMT;
    result->stmt = stmt;
    result->loc = loc;
    return result;
}

static inline AstNode *ast_node_new_decl(FileLocation loc, AstNodeDecl *decl) {
    AstNode *result = malloc(sizeof(AstNode));
    result->kind = AST_NODE_DECL;
    result->decl = decl;
    result->loc = loc;
    return result;
}
