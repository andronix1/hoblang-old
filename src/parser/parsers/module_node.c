#include "../parsers.h"
#include "decls/ext_func.c"
#include "decls/func.c"

bool parse_module_node(Parser *parser, AstModuleNode *node) {
	parser_next_token(parser);
	switch (token_type(parser->token)) {
		case TOKEN_FUN:
			node->type = AST_MODULE_NODE_FUNC;
			return parse_func_decl(parser, &node->func_decl);
		case TOKEN_EXTERN:
			node->type = AST_MODULE_NODE_EXTERNAL_FUNC;
			return parse_ext_func_decl(parser, &node->ext_func_decl);
		default:
			parse_err("unexpected `{tok}` while parsing module node", parser->token);
			return false;
	}
}
