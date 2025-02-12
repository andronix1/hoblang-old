#include "ast/private/stmts/return.h"
#include "parser/private.h"
#include "parser/token_stops.h"
#include "parser/parts/expr.h"

bool parse_return(Parser *parser, AstReturn *ret) {
	if (parser_next_is(parser, TOKEN_SEMICOLON)) {
		ret->has_value = false;
		return true;
	}
	ret->has_value = true;
	if (!(ret->expr = parse_expr(parser, token_stop_semicolon))) {
		return false;
	}
	return true;
}
