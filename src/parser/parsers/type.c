#include "../parsers.h"

bool parse_type(Parser *parser, AstType *type) {
	parser_next_token(parser);
	switch (token_type(parser->token)) {
		case TOKEN_IDENT:
			type->type = AST_TYPE_IDENT;
			type->ident = parser->token->ident;
			return true;
		default:
			parse_err(EXPECTED("type"));
			return false;
	}
}
