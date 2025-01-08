#include "../parsers.h"

bool parse_func_info(Parser *parser, AstFuncInfo *info) {
	parse_exp_next(TOKEN_IDENT, "function name");
	info->name = parser->token->ident;

	parse_exp_next(TOKEN_OPENING_CIRCLE_BRACE, "opening args brace");
	bool parsing_args = true;
	bool was_arg = false;
	info->args = vec_new(AstFuncArg);
	while (parsing_args) {
		parser_next_token(parser);
		switch (token_type(parser->token)) {
			case TOKEN_CLOSING_CIRCLE_BRACE:
				parsing_args = false;
				break;
			case TOKEN_COMMA:
				if (!was_arg) {
					parse_err("commas only after args allowed");
					return false;
				}
				was_arg = false;
				break;
			case TOKEN_IDENT: {
				AstFuncArg arg;
				arg.name = parser->token->ident;
				parse_exp_next(TOKEN_COLON, "arg type delimeter");
				if (!parse_type(parser, &arg.type)) {
					return false;
				}
				info->args = vec_push(info->args, &arg);
				was_arg = true;
				break;
			}
			default:
				parse_err(EXPECTED("function args"));
				return false;
		}
	}

	parse_exp_next(TOKEN_COLON, "returning type");
	parse_type(parser, &info->returning);
	return true;
}
