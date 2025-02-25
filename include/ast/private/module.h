#pragma once

#include "parser/parser.h"
#include "module_node.h"

typedef struct AstModule {
    const char *path;
	AstModuleNode *nodes;
    Parser *parser;
} AstModule;

