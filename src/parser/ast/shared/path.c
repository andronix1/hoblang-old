#include "path.h"
#include "ast/shared/path.h"
#include "core/not_null.h"
#include "lexer/token/token.h"
#include "core/vec.h"
#include "parser/api.h"
#include "parser/ast/shared/type.h"

AstPath *parse_path(Parser *parser) {
    AstPathSegment *segments = vec_new(AstPathSegment);
    do {
        Token *token = parser_next(parser);
        FileLocation loc = token->location;
        switch (token->kind) {
            case TOKEN_IDENT:
                segments = vec_push_dir(segments, ast_path_segment_new_ident(loc, token->ident));
                break;
            case TOKEN_MULTIPLY:
                segments = vec_push_dir(segments, ast_path_segment_new_deref(loc));
                break;
            case TOKEN_NULL:
                segments = vec_push_dir(segments, ast_path_segment_new_null(loc));
                break;
            case TOKEN_SIZEOF:
                segments = vec_push_dir(segments, ast_path_segment_new_sizeof(loc));
                break;
            case TOKEN_LESS: {
                AstType **params = vec_new(AstType*);
                while (parser_next_is_not(parser, TOKEN_GREATER)) {
                    params = vec_push_dir(params, NOT_NULL(parse_type(parser)));
                    switch (parser_next(parser)->kind) {
                        case TOKEN_COMMA: break;
                        case TOKEN_GREATER:
                            parser_skip_next(parser);
                            break;
                        default:
                            PARSE_ERROR(EXPECTED("comma or >"));
                            return NULL;
                    }
                }
                segments = vec_push_dir(segments, ast_path_segment_new_generic(loc, params));
                break;
            }
            default: 
                PARSE_ERROR("unkown path segment");
                return NULL;
        }
    } while (parser_next_is(parser, TOKEN_DOT));
    return ast_path_new(segments);
}

