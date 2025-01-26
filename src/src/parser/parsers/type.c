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
			return parse_mod_path(parser, &type->path);
		case TOKEN_MULTIPLY: {
			type->type = AST_TYPE_POINTER;
			type->ptr_to = malloc(sizeof(AstType));
			return parse_type(parser, type->ptr_to);
		}
		case TOKEN_STRUCT: {
			type->type = AST_TYPE_STRUCT;
			return parse_ast_struct_type(parser, &type->struct_type);
		}
		case TOKEN_FUN: {
			type->type = AST_TYPE_FUNCTION;
			return parse_ast_func_type(parser, &type->func_type);
		}
		default:
			parse_err(EXPECTED("type"));
			return false;
	}
}
