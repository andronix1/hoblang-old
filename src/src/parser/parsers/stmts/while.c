#include "../../parsers.h"

bool parse_while(Parser *parser, AstWhile *while_loop) {
	if (!parse_expr(parser, &while_loop->expr, token_opening_figure_brace_stop)) {
		return false;
	}
	parser->skip_next = true;
	return parse_body(parser, while_loop->body = malloc(sizeof(AstBody)));
}
