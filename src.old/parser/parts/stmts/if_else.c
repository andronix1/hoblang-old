#include <malloc.h>
#include "core/vec.h"
#include "parser/private.h"
#include "parser/token_stops.h"
#include "parser/parts/expr.h"
#include "parser/parts/body.h"
#include "ast/private/stmts/if_else.h"
#include "ast/private/body.h"

bool parse_if_else_body(Parser *parser, AstIfBody *body) {
	if (!(body->expr = parse_expr(parser, token_stop_opening_figure_brace))) {
		return false;
	}
	parser_skip_next(parser);
	return parse_body(parser, body->body = malloc(sizeof(AstBody)));
}

bool parse_if_else(Parser *parser, AstIfElse *if_else) {
	if_else->else_ifs = vec_new(AstIfBody);
	if_else->has_else = false;
	if (!parse_if_else_body(parser, &if_else->main)) {
		return false;
	}
	while (parser_next_is(parser, TOKEN_ELSE)) {
		switch (parser_next(parser)->type) {
			case TOKEN_IF: {
				AstIfBody body;
				if (!parse_if_else_body(parser, &body)) {
					return false;
				}
				if_else->else_ifs = vec_push(if_else->else_ifs, &body);
				break;
			}
			case TOKEN_OPENING_FIGURE_BRACE:
				parser_skip_next(parser);
				if (!parse_body(parser, if_else->else_body = malloc(sizeof(AstBody)))) {
					return false;
				}
				if_else->has_else = true;
				return true;
			default:
				PARSE_ERROR("expected else if or else body but got `{tok}`", parser_token(parser));
				return false;
		}
	}
	return true;
}
