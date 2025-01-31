#include "../parsers.h"

bool parse_decl_path(Parser *parser, AstDeclPath *path) {
    path->segments = vec_new(Slice);
	while (true) {
        parse_exp_next(TOKEN_IDENT, "module decl name");
		path->segments = vec_push(path->segments, &parser->token->ident);
		parser_next_token(parser);
		if (token_type(parser->token) != TOKEN_DOUBLE_COLON) {
			parser->skip_next = true;
			return true;
		}
	}
}

bool parse_inner_path(Parser *parser, AstInnerPath *path) {
    path->segments = vec_new(AstInnerPathSegment);
    while (true) {
		parser_next_token(parser);
		AstInnerPathSegment segment;
		switch (token_type(parser->token)) {
			case TOKEN_IDENT:
				segment.type = AST_INNER_PATH_SEG_IDENT;
				segment.ident = parser->token->ident;
				break;
			default:
				parse_err(EXPECTED("inner path segment"));
				return false;
		}
        path->segments = vec_push(path->segments, &segment);
        parser_next_token(parser);
        if (token_type(parser->token) != TOKEN_DOT) {
            parser->skip_next = true;
            return true;
        }
    }
}

bool parse_path(Parser *parser, AstPath *path) {
	if (!parse_decl_path(parser, &path->decl_path)) {
		return false;
	}
	parser_next_token(parser);
	if (token_type(parser->token) != TOKEN_DOT) {
		path->inner_path.segments = vec_new(AstInnerPathSegment);
		return true;
	}
	return parse_inner_path(parser, &path->inner_path);

}
