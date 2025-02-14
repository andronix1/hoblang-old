#include "ast/private/type.h"
#include "parser/private.h"
#include "parser/token_stops.h"
#include "parser/parts/type.h"
#include "parser/parts/expr.h"
#include "parser/parts/path.h"
#include "parser/parts/types.h"

bool parse_type(Parser *parser, AstType *type) {
	type->sema = NULL;
	Token *token = parser_next(parser);
	switch (token->type) {
		case TOKEN_IDENT:
			type->type = AST_TYPE_PATH;
			parser_skip_next(parser);
			return parse_path(parser, &type->path);
		case TOKEN_MULTIPLY: {
			type->type = AST_TYPE_POINTER;
			return parse_type(parser, type->ptr_to = malloc(sizeof(AstType)));
		}
		case TOKEN_FUN: {
			type->type = AST_TYPE_FUNCTION;
			return parse_ast_func_type(parser, &type->func);
		}
		case TOKEN_OPENING_SQUARE_BRACE: {
			if (parser_next_is(parser, TOKEN_CLOSING_SQUARE_BRACE)) {
				type->type = AST_TYPE_SLICE;
				return parse_type(parser, type->slice_of = malloc(sizeof(AstType)));
			}
			type->type = AST_TYPE_ARRAY;
			type->array.length = parse_expr(parser, token_stop_idx);
			if (!type->array.length) {
				return false;
			}
			PARSER_EXPECT_NEXT(TOKEN_CLOSING_SQUARE_BRACE, "array length closing brace");
			return parse_type(parser, type->array.of = malloc(sizeof(AstType)));
		}
		default:
			PARSE_ERROR(EXPECTED("type"));
			return false;
	}
}
