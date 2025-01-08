#pragma once

#include "module_nodes/func_decl.h"
#include "module_nodes/ext_func_decl.h"

typedef enum {
	AST_MODULE_NODE_FUNC,
	AST_MODULE_NODE_EXTERNAL_FUNC,
} AstModuleNodeType;

typedef struct {
	AstModuleNodeType type;
	union {
		AstFuncDecl func_decl;		
		AstExtFuncDecl ext_func_decl;		
	};
} AstModuleNode;

void print_ast_module_node(FILE *stream, va_list *list);
