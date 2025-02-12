#include "parser/private.h"
#include "parser/token_stops.h"
#include "ast/private/module_node.h"
#include "ast/private/module.h"
#include "parser/parts/module_node.h"

bool parse_module(Parser *parser, AstModule *module) {
	module->nodes = vec_new(AstModuleNode);
	while (true) {
		AstModuleNode node;
		if (parser_next_is(parser, TOKEN_EOI)) {
			return true;
		}
		if (!parse_module_node(parser, &node)) {
			return false;
		}
		module->nodes = vec_push(module->nodes, &node);
	}
	return true;
}
