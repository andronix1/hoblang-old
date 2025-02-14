#include <stdbool.h>
#include "parser/parser.h"
#include "ast/api/module_node.h"

bool parse_module_node(Parser *parser, AstModuleNode *node);
bool parse_ast_struct_def(Parser *parser, AstStructDef *struct_type);
