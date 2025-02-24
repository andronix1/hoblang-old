#include "lexer/token.h"
#include "parser/private.h"
#include "parser/parts/type.h"
#include "ast/private/type.h"

bool parse_ast_func_type(Parser *parser, AstFunctionType *func_type) {
	PARSER_EXPECT_NEXT(TOKEN_OPENING_CIRCLE_BRACE, "function args");
	func_type->args = vec_new(AstType);
    bool reading = !parser_next_is(parser, TOKEN_CLOSING_CIRCLE_BRACE);
	while (reading) {
		AstType type;
		if (!parse_type(parser, &type)) {
			return false;
		}
		func_type->args = vec_push(func_type->args, &type);
		switch (parser_next(parser)->type) {
			case TOKEN_CLOSING_CIRCLE_BRACE:
                reading = false;
                break;
			case TOKEN_COMMA:
				break;
			default:
				PARSE_ERROR(EXPECTED("argument type"));
				return false;
		}
	}
    PARSER_EXPECT_NEXT(TOKEN_FUNC_RETURNS, "function returning type");
    return parse_type(parser, func_type->returns = malloc(sizeof(AstType)));
}
