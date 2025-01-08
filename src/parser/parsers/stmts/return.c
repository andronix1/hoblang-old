#include "../../parsers.h"

bool parse_return(Parser *parser, AstReturn *ret) {
	parser_next_token(parser);
	if (token_type(parser->token) == TOKEN_SEMICOLON) {
		ret->has_value = false;
		return true;
	}
	ret->has_value = true;
	parser->skip_next = true;
	return parse_expr(parser, &ret->expr, token_semicolon_stop);
}
