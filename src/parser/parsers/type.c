#include "../parsers.h"

bool parse_type(Parser *parser, AstType *type) {
	parser_next_token(parser);
	type->sema = NULL;
	switch (token_type(parser->token)) {
		case TOKEN_IDENT:
			type->type = AST_TYPE_IDENT;
			type->ident = parser->token->ident;
			break;
		default:
			parse_err(EXPECTED("type"));
			return false;
	}
	while (true) {
		parser_next_token(parser);
		switch (parser->token->type) {
			case TOKEN_MULTIPLY: {
				AstType *cloned = malloc(sizeof(AstType));
				memcpy(cloned, type, sizeof(AstType));
				type->type = AST_TYPE_POINTER;
				type->ptr_to = cloned;
				break;
			}
			default:
				parser->skip_next = true;
				return true;
		}
	}
}
