#include "ast/private/func_info.h"
#include "parser/parts/func_info.h"
#include "parser/parts/type.h"
#include "lexer/token.h"
#include "parser/private.h"

bool parse_func_type(Parser *parser, AstFuncArg **args, AstType *returning) {
	PARSER_EXPECT_NEXT(TOKEN_OPENING_CIRCLE_BRACE, "opening args brace");
	bool parsing_args = true;
	bool was_arg = false;
	*args = vec_new(AstFuncArg);
	while (parsing_args) {
		Token *token = parser_next(parser);
		switch (token->type) {
			case TOKEN_CLOSING_CIRCLE_BRACE:
				parsing_args = false;
				break;
			case TOKEN_COMMA:
				if (!was_arg) {
					PARSE_ERROR("commas only after args allowed");
					return false;
				}
				was_arg = false;
				break;
			case TOKEN_IDENT: {
				AstFuncArg arg;
				arg.name = token->ident;
                arg.loc = token->location;
				PARSER_EXPECT_NEXT(TOKEN_COLON, "arg type delimeter");
				if (!parse_type(parser, &arg.type)) {
					return false;
				}
				*args = vec_push(*args, &arg);
				was_arg = true;
				break;
			}
			default:
				PARSE_ERROR(EXPECTED("function args"));
				return false;
		}
	}

	PARSER_EXPECT_NEXT(TOKEN_FUNC_RETURNS, "returning type");
	return parse_type(parser, returning);
}

bool parse_func_info(Parser *parser, AstFuncInfo *info) {
	if ((info->is_extension = parser_next_is(parser, TOKEN_OPENING_CIRCLE_BRACE))) {
        if (!parse_type(parser, &info->ext.of)) {
            return false;
        }
		PARSER_EXPECT_NEXT(TOKEN_CLOSING_CIRCLE_BRACE, "extension type closing");
	}
	info->name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "function name")->ident;
	return parse_func_type(parser, &info->args, &info->returning);
}
