#include "../parsers.h"

bool parse_value(Parser *parser, AstValue *value) {
	value->segments = vec_new(AstValueSegment);
	while (true) {
		parser_next_token(parser);
		AstValueSegment segment;
		switch (token_type(parser->token)) {
			case TOKEN_IDENT:
				segment.type = AST_VALUE_IDENT;
				segment.ident = parser->token->ident;
				break;
			case TOKEN_MULTIPLY:
				segment.type = AST_VALUE_DEREF;
				break;
			case TOKEN_REF:
				segment.type = AST_VALUE_REF;
				break;
			default:
				parse_err("expected value got {tok}", parser->token);
				return false;
		}
		value->segments = vec_push(value->segments, &segment);
		parser_next_token(parser);
		if (token_type(parser->token) != TOKEN_DOT) {
			value->sema_type = segment.sema_type;
			parser->skip_next = true;
			return true;
		}
	}
}
