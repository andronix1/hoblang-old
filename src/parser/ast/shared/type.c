#include "type.h"
#include "ast/interface/type.h"
#include "core/not_null.h"
#include "core/vec.h"
#include "lexer/token/kind.h"
#include "parser/api.h"
#include "ast/shared/type.h"
#include "parser/ast/shared/expr.h"
#include "parser/ast/shared/path.h"

AstType *parse_type(Parser *parser) {
    switch (parser_next(parser)->kind) {
        case TOKEN_IDENT:
            parser_skip_next(parser);
            return ast_type_new_path(NOT_NULL(parse_path(parser)));
        case TOKEN_OPENING_SQUARE_BRACE: {
            if (parser_next_is(parser, TOKEN_CLOSING_SQUARE_BRACE)) {
                return ast_type_new_slice(NOT_NULL(parse_type(parser)));
            }
            AstExpr *length = NOT_NULL(parse_expr(parser));
            PARSER_EXPECT_NEXT(TOKEN_CLOSING_SQUARE_BRACE, "]");
            return ast_type_new_array(NOT_NULL(parse_type(parser)), length);
        }
        case TOKEN_MULTIPLY: return ast_type_new_pointer(NOT_NULL(parse_type(parser)));
        case TOKEN_QUESTION_MARK: return ast_type_new_optional(NOT_NULL(parse_type(parser)));
        case TOKEN_FUN: {
            PARSER_EXPECT_NEXT(TOKEN_OPENING_CIRCLE_BRACE, "(");
            AstType **args = vec_new(AstType*);
            while (parser_next_is_not(parser, TOKEN_CLOSING_CIRCLE_BRACE)) {
                args = vec_push_dir(args, NOT_NULL(parse_type(parser)));
                switch (parser_next(parser)->kind) {
                    case TOKEN_COMMA: break;
                    case TOKEN_CLOSING_CIRCLE_BRACE:
                        parser_skip_next(parser);
                        break;
                    default:
                        PARSE_ERROR(EXPECTED("comma or )"));
                        return NULL;
                }
            }
            PARSER_EXPECT_NEXT(TOKEN_FUNC_RETURNS, "->");
            AstType *returns = NOT_NULL(parse_type(parser));
            return ast_type_new_func(args, returns);
        }
        default:
            PARSE_ERROR(EXPECTED("type"));
            return NULL;
    }
}

