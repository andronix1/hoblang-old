#include "../../parts.h"
#include "../expr.h"
#include "ast/private/stmts/while.h"
#include "ast/private/body.h"
#include "../body.h"

bool parse_while(Parser *parser, AstWhile *while_loop) {
	if (!(while_loop->expr = parse_expr(parser, token_opening_figure_brace_stop))) {
		return false;
	}
	parser_skip_next(parser);
	return parse_body(parser, while_loop->body = malloc(sizeof(AstBody)));
}
