#include "../parsers.h"

bool parse_path(Parser *parser, AstPath *path) {
    path->segments = vec_new(AstPathSegment);
    while (true) {
        parse_exp_next(TOKEN_IDENT, "ident");
        AstPathSegment segment = {
            .type = AST_PATH_SEGMENT_IDENT,
            .name = parser->token->ident
        };
        path->segments = vec_push(path->segments, &segment);
        parser_next_token(parser);
        if (token_type(parser->token) != TOKEN_DOT) {
            parser->skip_next = true;
            return true;
        }
    }
}
