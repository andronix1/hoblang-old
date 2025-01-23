#include "../../parsers.h"

bool parse_ast_struct_member(Parser *parser, AstStructMember *member) {
	parse_exp_next(TOKEN_IDENT, "struct member name");
	member->name = parser->token->ident;
	parse_exp_next(TOKEN_COLON, "colon");
	return parse_type(parser, member->type = malloc(sizeof(AstType)));
}

bool parse_ast_struct_type(Parser *parser, AstStructType *struct_type) {
	parse_exp_next(TOKEN_OPENING_FIGURE_BRACE, "struct content open");
	struct_type->members = vec_new(AstStructMember);
	while (true) {
		AstStructMember member;
		if (!parse_ast_struct_member(parser, &member)) {
			return false;
		}
		struct_type->members = vec_push(struct_type->members, &member);
		parser_next_token(parser);
		switch (token_type(parser->token)) {
			case TOKEN_CLOSING_FIGURE_BRACE:
				return true;
			case TOKEN_COMMA:
				break;
			default:
				parse_err(EXPECTED("struct"));
				return false;
		}
	}
}
