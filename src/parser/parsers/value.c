#include "../parsers.h"

bool parse_value(Parser *parser, AstValue *value) {
	value->segments = vec_new(AstValueSegment);
	if (!parse_mod_path(parser, &value->mod_path)) {
		return false;
	}
	while (true) {
		bool next = false;
		while (!next) {
			parser_next_token(parser);
			switch (token_type(parser->token)) {
				case TOKEN_DOT: next = true; break;
				case TOKEN_OPENING_SQUARE_BRACE: {
					AstValueSegment idx_seg;
					idx_seg.type = AST_VALUE_IDX;
					if (!(idx_seg.idx.expr = parse_expr_before(parser, token_idx_stop))) {
						return false;
					}
					value->segments = vec_push(value->segments, &idx_seg);
					break;
				}
				default:
					parser->skip_next = true;
					return true;
			}
		}
		parser_next_token(parser);
		AstValueSegment segment;
		switch (token_type(parser->token)) {
			case TOKEN_IDENT:
				segment.type = AST_VALUE_IDENT;
				segment.ident.ident = parser->token->ident;
				break;
			case TOKEN_MULTIPLY:
				segment.type = AST_VALUE_DEREF;
				break;
			default:
				parse_err("expected value got {tok}", parser->token);
				return false;
		}
		value->segments = vec_push(value->segments, &segment);
	}
}
