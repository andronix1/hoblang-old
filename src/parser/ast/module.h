#pragma once

#include "core/vec.h"
#include "module_node.h"

typedef struct _AstModule {
	Slice name;
	AstModuleNode *nodes;
} AstModule;

void print_ast_module(FILE *stream, va_list *list);
