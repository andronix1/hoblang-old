#include "ast/impl/type.h"
#include "lexer/token.h"
#include "parser/private.h"
#include "parser/parts/type.h"
#include "core/vec.h"

AstType *parse_ast_func_type(Parser *parser) {
	PARSER_EXPECT_NEXT(TOKEN_OPENING_CIRCLE_BRACE, "function args");
	AstType **args = vec_new(AstType*);
    bool reading = !parser_next_is(parser, TOKEN_CLOSING_CIRCLE_BRACE);
	while (reading) {
		AstType *arg_type = parse_type(parser);
		if (!arg_type) {
			return NULL;
		}
		args = vec_push(args, &arg_type);
		switch (parser_next(parser)->type) {
			case TOKEN_CLOSING_CIRCLE_BRACE:
                reading = false;
                break;
			case TOKEN_COMMA:
				break;
			default:
				PARSE_ERROR(EXPECTED("argument type"));
				return NULL;
		}
	}
    PARSER_EXPECT_NEXT(TOKEN_FUNC_RETURNS, "function returning type");
    AstType *returns = parse_type(parser);
    if (!returns) {
        return NULL;
    }
    return ast_type_new_func(args, returns);
}
