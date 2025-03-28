#include "lexer/token.h"
#include "parser/private.h"
#include "parser/token_stops.h"
#include "parser/parts/expr.h"
#include "ast/private/stmts/while.h"
#include "ast/private/body.h"
#include "parser/parts/body.h"
#include <malloc.h>

bool parse_while(Parser *parser, AstWhile *while_loop) {
    Token *token = parser_next_is_or(parser, TOKEN_OPENING_SQUARE_BRACE);
    while_loop->is_named = token != NULL;
    if (token) {
        while_loop->name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "loop name")->ident;
        PARSER_EXPECT_NEXT(TOKEN_CLOSING_SQUARE_BRACE, "closing figure brace");
    }
	if (!(while_loop->expr = parse_expr(parser, token_stop_opening_figure_brace))) {
		return false;
	}
	parser_skip_next(parser);
	return parse_body(parser, while_loop->body = malloc(sizeof(AstBody)));
}
