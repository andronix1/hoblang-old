#pragma once

#include "core/vec.h"
#include "module_node.h"

typedef struct _AstModule {
	AstModuleNode *nodes;
} AstModule;

void print_ast_module(FILE *stream, va_list *list);
