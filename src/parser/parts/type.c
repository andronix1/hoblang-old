#include "ast/api/type.h"
#include "ast/impl/type.h"
#include "parser/private.h"
#include "parser/token_stops.h"
#include "parser/parts/type.h"
#include "parser/parts/expr.h"
#include "parser/parts/path.h"
#include "parser/parts/types.h"

AstType *parse_type(Parser *parser) {
	Token *token = parser_next(parser);
	switch (token->type) {
		case TOKEN_QUESTION_MARK: {
            AstType *of = parse_type(parser); 
            if (!of) {
                return NULL;
            }
			return ast_type_new_optional(of);
        }
		case TOKEN_IDENT: {
			parser_skip_next(parser);
			AstPath *path = parse_path(parser);
            if (!path) {
                return NULL;
            }
            return ast_type_new_path(path);
        }
		case TOKEN_MULTIPLY: {
            AstType *to = parse_type(parser); 
            if (!to) {
                return NULL;
            }
			return ast_type_new_pointer(to);
		}
		case TOKEN_FUN: {
            return parse_ast_func_type(parser);
		}
		case TOKEN_OPENING_SQUARE_BRACE: {
			if (parser_next_is(parser, TOKEN_CLOSING_SQUARE_BRACE)) {
                AstType *of = parse_type(parser); 
                if (!of) {
                    return NULL;
                }
				return ast_type_new_slice(of);
			}
			AstExpr *length = parse_expr(parser, token_stop_idx);
			if (!length) {
				return false;
			}
			PARSER_EXPECT_NEXT(TOKEN_CLOSING_SQUARE_BRACE, "array length closing brace");
            AstType *of = parse_type(parser);
            if (!of) {
                return NULL;
            }
			return ast_type_new_array(of, length);
		}
		default:
			PARSE_ERROR(EXPECTED("type"));
			return NULL;
	}
}
