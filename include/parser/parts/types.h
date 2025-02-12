#include <stdbool.h>
#include "ast/api/type.h"
#include "parser/parser.h"

bool parse_ast_func_type(Parser *parser, AstFunctionType *func_type);
bool parse_ast_struct_type(Parser *parser, AstStructType *struct_type);

