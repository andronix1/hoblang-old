#include "../../parts.h"
#include "../expr.h"

bool parse_return(Parser *parser, AstReturn *ret) {
	if (parser_next_is(parser, TOKEN_SEMICOLON)) {
		ret->has_value = false;
		return true;
	}
	ret->has_value = true;
	if (!(ret->expr = parse_expr(parser, token_semicolon_stop))) {
		return false;
	}
	return true;
}
