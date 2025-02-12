#include "parser/private.h"
#include "parser/token_stops.h"
#include "parser/parts/expr.h"
#include "ast/private/stmts/while.h"
#include "ast/private/body.h"
#include "parser/parts/body.h"

bool parse_while(Parser *parser, AstWhile *while_loop) {
	if (!(while_loop->expr = parse_expr(parser, token_stop_opening_figure_brace))) {
		return false;
	}
	parser_skip_next(parser);
	return parse_body(parser, while_loop->body = malloc(sizeof(AstBody)));
}
