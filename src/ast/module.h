#pragma once

#include <malloc.h>
#include "ast/interface/node.h"
#include "parser/interface/parser.h"

typedef struct AstModule {
    AstNode **nodes;
    Parser *parser;
} AstModule;

static inline AstModule *ast_module_new(AstNode **nodes, Parser *parser) {
    AstModule *module = malloc(sizeof(AstModule));
    module->nodes = nodes;
    module->parser = parser;
    return module;
}
