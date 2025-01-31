#include "../parsers.h"
#include "types/struct.c"
#include "types/func.c"

bool parse_type(Parser *parser, AstType *type) {
	parser_next_token(parser);
	type->sema = NULL;
	switch (token_type(parser->token)) {
		case TOKEN_IDENT:
			type->type = AST_TYPE_PATH;
			parser->skip_next = true;
			return parse_path(parser, &type->path);
		case TOKEN_MULTIPLY: {
			type->type = AST_TYPE_POINTER;
			return parse_type(parser, type->ptr_to = malloc(sizeof(AstType)));
		}
		case TOKEN_STRUCT: {
			type->type = AST_TYPE_STRUCT;
			return parse_ast_struct_type(parser, &type->struct_type);
		}
		case TOKEN_FUN: {
			type->type = AST_TYPE_FUNCTION;
			return parse_ast_func_type(parser, &type->func);
		}
		case TOKEN_OPENING_SQUARE_BRACE: {
			parser_next_token(parser);
			if (token_type(parser->token) == TOKEN_INTEGER) {
				type->type = AST_TYPE_ARRAY;
				type->array.length = parser->token->integer;
				parse_exp_next(TOKEN_CLOSING_SQUARE_BRACE, "array length closing brace");
				return parse_type(parser, type->array.of = malloc(sizeof(AstType)));
			} else {
				parse_exp(TOKEN_CLOSING_SQUARE_BRACE, "slice closing brace");
				type->type = AST_TYPE_SLICE;
				return parse_type(parser, type->slice_of = malloc(sizeof(AstType)));
			}
		}
		default:
			parse_err(EXPECTED("type"));
			return false;
	}
}
