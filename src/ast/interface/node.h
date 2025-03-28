#pragma once

#include "core/slice/slice.h"

typedef struct AstNodeDeclGeneric AstNodeDeclGeneric;
typedef struct AstNodeDecl AstNodeDecl;
typedef struct AstNodeStmt AstNodeStmt;
typedef struct AstNode AstNode;

Slice ast_node_decl_generic_get_name(AstNodeDeclGeneric *decl);
