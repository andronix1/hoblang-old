#include "../../parsers.h"

bool parse_if_else_body(Parser *parser, AstIfBody *body) {
	if (!parse_expr(parser, &body->expr, token_opening_figure_brace_stop)) {
		return false;
	}
	parser->skip_next = true;
	return parse_body(parser, body->body = malloc(sizeof(AstBody)));
}

bool parse_if_else(Parser *parser, AstIfElse *if_else) {
	if_else->else_ifs = vec_new(AstIfBody);
	if_else->has_else = false;
	if (!parse_if_else_body(parser, &if_else->main)) {
		return false;
	}
	parser_next_token(parser);
	while (token_type(parser->token) == TOKEN_ELSE) {
		parser_next_token(parser);
		switch (token_type(parser->token)) {
			case TOKEN_IF: {
				AstIfBody body;
				if (!parse_if_else_body(parser, &body)) {
					return false;
				}
				if_else->else_ifs = vec_push(if_else->else_ifs, &body);
				break;
			}
			case TOKEN_OPENING_FIGURE_BRACE:
				parser->skip_next = true;
				if (!parse_body(parser, if_else->else_body = malloc(sizeof(AstBody)))) {
					return false;
				}
				if_else->has_else = true;
				return true;
			default:
				parse_err("expected else if or else body but got `{tok}`", parser->token);
				return false;
		}
		parser_next_token(parser);
	}
	parser->skip_next = true;
	return true;
}
