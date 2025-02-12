#include "parser/private.h"
#include "parser/parts/type.h"
#include "ast/private/type.h"

bool parse_ast_struct_member(Parser *parser, AstStructMember *member) {
	member->name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "struct member name")->ident;
	PARSER_EXPECT_NEXT(TOKEN_COLON, "colon");
	return parse_type(parser, member->type = malloc(sizeof(AstType)));
}

bool parse_ast_struct_type(Parser *parser, AstStructType *struct_type) {
	PARSER_EXPECT_NEXT(TOKEN_OPENING_FIGURE_BRACE, "struct content open");
	struct_type->members = vec_new(AstStructMember);
	while (true) {
		AstStructMember member;
		if (!parse_ast_struct_member(parser, &member)) {
			return false;
		}
		struct_type->members = vec_push(struct_type->members, &member);
		switch (parser_next(parser)->type) {
			case TOKEN_CLOSING_FIGURE_BRACE:
				return true;
			case TOKEN_COMMA:
				break;
			default:
				PARSE_ERROR(EXPECTED("struct"));
				return false;
		}
	}
}
