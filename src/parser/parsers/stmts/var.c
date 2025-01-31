#include "../../parsers.h"

bool parse_var(Parser *parser, AstVar *var) {
	parse_exp_next(TOKEN_IDENT, "variable name");
	var->name = parser->token->ident;
	parser_next_token(parser);
	var->typed = false;
	switch (token_type(parser->token)) {
		case TOKEN_SEMICOLON:
			return true;
		case TOKEN_ASSIGN:
			var->initializes = true;
			if (!(var->expr = parse_expr(parser, token_semicolon_stop))) {
				return false;
			}
			return true;
		case TOKEN_COLON:
			var->typed = true;
			if (!parse_type(parser, &var->type)) {
				return false;
			}
			break;
		default:
			parse_err("expected type or assign but got `{tok}`", parser->token);
			return false;
	}
	parser_next_token(parser);
	switch (token_type(parser->token)) {
		case TOKEN_ASSIGN:
			var->initializes = true;
			if (!(var->expr = parse_expr(parser, token_semicolon_stop))) {
				return false;
			}
			return true;
		case TOKEN_SEMICOLON:
			var->initializes = false;
			return true;
		default:
			parse_err("expected initilizer or end but got `{tok}`", parser->token);
			return false;
	}
}
