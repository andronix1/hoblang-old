#include "../../parsers.h"

bool parse_ast_func_type(Parser *parser, AstFunctionType *func_type) {
	parse_exp_next(TOKEN_OPENING_CIRCLE_BRACE, "function args");
	func_type->args = vec_new(AstType);
	while (true) {
		AstType type;
		if (!parse_type(parser, &type)) {
			return false;
		}
		func_type->args = vec_push(func_type->args, &type);
		parser_next_token(parser);
		switch (token_type(parser->token)) {
			case TOKEN_CLOSING_CIRCLE_BRACE:
				parse_exp_next(TOKEN_FUNC_RETURNS, "function returning type");
				return parse_type(parser, func_type->returns = malloc(sizeof(AstType)));
			case TOKEN_COMMA:
				break;
			default:
				parse_err(EXPECTED("argument type"));
				return false;
		}
	}
}
