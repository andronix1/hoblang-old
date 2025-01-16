#include "../parsers.h"

bool parse_mod_path(Parser *parser, AstModPath *mod_path) {
	mod_path->segments = vec_new(Slice);
	parse_exp_next(TOKEN_IDENT, "ident");
	mod_path->segments = vec_push(mod_path->segments, &parser->token->ident);
	parser_next_token(parser);
	while (token_type(parser->token) != TOKEN_EOI && token_type(parser->token) == TOKEN_DOUBLE_COLON) {
		parse_exp_next(TOKEN_IDENT, "ident");
		mod_path->segments = vec_push(mod_path->segments, &parser->token->ident);
		parser_next_token(parser);
	}
	parser->skip_next = true;
	return true;
}
