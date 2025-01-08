#include "../parsers.h"

bool parse_module(Parser *parser, AstModule *module) {
	module->nodes = vec_new(AstModuleNode);
	while (true) {
		AstModuleNode node;
		parser_next_token(parser);
		if (token_type(parser->token) == TOKEN_EOI) {
			return true;
		}
		parser->skip_next = true;
		if (!parse_module_node(parser, &node)) {
			return false;
		}
		module->nodes = vec_push(module->nodes, &node);
	}
	return true;
}
