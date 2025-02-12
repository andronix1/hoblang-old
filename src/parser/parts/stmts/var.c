#include "ast/private/stmts/var.h"
#include "parser/private.h"
#include "parser/token_stops.h"
#include "parser/parts/expr.h"
#include "parser/parts/type.h"

bool parse_var(Parser *parser, AstVar *var) {
	var->name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "variable name")->ident;
	var->typed = false;
	switch (parser_next(parser)->type) {
		case TOKEN_SEMICOLON:
			return true;
		case TOKEN_ASSIGN:
			var->initializes = true;
			if (!(var->expr = parse_expr(parser, token_stop_semicolon))) {
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
			PARSE_ERROR("expected type or assign but got `{tok}`", parser_token(parser));
			return false;
	}
	switch (parser_next(parser)->type) {
		case TOKEN_ASSIGN:
			var->initializes = true;
			if (!(var->expr = parse_expr(parser, token_stop_semicolon))) {
				return false;
			}
			return true;
		case TOKEN_SEMICOLON:
			var->initializes = false;
			return true;
		default:
			PARSE_ERROR("expected initilizer or end but got `{tok}`", parser_token(parser));
			return false;
	}
}
