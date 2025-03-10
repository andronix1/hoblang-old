#include "ast/private/func_info.h"
#include "parser/parts/func_info.h"
#include "parser/parts/type.h"
#include "lexer/token.h"
#include "parser/private.h"

bool parse_func_type_info(Parser *parser, AstFuncTypeInfo *info) {
	PARSER_EXPECT_NEXT(TOKEN_OPENING_CIRCLE_BRACE, "opening args brace");
	bool parsing_args = true;
	bool was_arg = false;
	info->args = vec_new(AstFuncArg);
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
				if (!(arg.type = parse_type(parser))) {
					return false;
				}
				info->args = vec_push(info->args, &arg);
				was_arg = true;
				break;
			}
			default:
				PARSE_ERROR(EXPECTED("function args"));
				return false;
		}
	}

	PARSER_EXPECT_NEXT(TOKEN_FUNC_RETURNS, "returning type");
	return (info->returning = parse_type(parser));
}

bool parse_func_info(Parser *parser, AstFuncInfo *info) {
	if ((info->is_extension = parser_next_is(parser, TOKEN_OPENING_CIRCLE_BRACE))) {
        
        if (!(info->ext.of = parse_type(parser))) {
            return false;
        }
		PARSER_EXPECT_NEXT(TOKEN_CLOSING_CIRCLE_BRACE, "extension type closing");
	}
	info->name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "function name")->ident;
	return parse_func_type_info(parser, &info->type);
}
