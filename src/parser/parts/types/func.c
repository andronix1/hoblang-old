#include "../../parts.h"

bool parse_ast_func_type(Parser *parser, AstFunctionType *func_type) {
	PARSER_EXPECT_NEXT(TOKEN_OPENING_CIRCLE_BRACE, "function args");
	func_type->args = vec_new(AstType);
	while (true) {
		AstType type;
		if (!parse_type(parser, &type)) {
			return false;
		}
		func_type->args = vec_push(func_type->args, &type);
		switch (parser_next(parser)->type) {
			case TOKEN_CLOSING_CIRCLE_BRACE:
				PARSER_EXPECT_NEXT(TOKEN_FUNC_RETURNS, "function returning type");
				return parse_type(parser, func_type->returns = malloc(sizeof(AstType)));
			case TOKEN_COMMA:
				break;
			default:
				PARSE_ERROR(EXPECTED("argument type"));
				return false;
		}
	}
}
