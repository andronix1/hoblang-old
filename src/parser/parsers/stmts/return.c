#include "../../parsers.h"

bool parse_return(Parser *parser, AstReturn *ret) {
	parser_next_token(parser);
	if (token_type(parser->token) == TOKEN_SEMICOLON) {
		ret->has_value = false;
		return true;
	}
	ret->has_value = true;
	parser->skip_next = true;
	if (!(ret->expr = parse_expr(parser, token_semicolon_stop))) {
		return false;
	}
	return true;
}
