#include "path.h"

bool parse_decl_path(Parser *parser, AstDeclPath *path) {
    path->segments = vec_new(Slice);
	while (true) {
		path->segments = vec_push(
			path->segments,
			&PARSER_EXPECT_NEXT(TOKEN_IDENT, "module decl name")->ident
		);
		if (parser_next_is_not(parser, TOKEN_DOUBLE_COLON)) {
			return true;
		}
	}
}

bool parse_inner_path(Parser *parser, AstInnerPath *path) {
    path->segments = vec_new(AstInnerPathSegment);
    while (true) {
		Token *token = parser_next(parser);
		AstInnerPathSegment segment;
		switch (token->type) {
			case TOKEN_IDENT:
				segment.type = AST_INNER_PATH_SEG_IDENT;
				segment.ident = token->ident;
				break;
			case TOKEN_MULTIPLY:
				segment.type = AST_INNER_PATH_SEG_DEREF;
				break;
			default:
				PARSE_ERROR(EXPECTED("inner path segment"));
				return false;
		}
        path->segments = vec_push(path->segments, &segment);
        if (parser_next_is_not(parser, TOKEN_DOT)) {
            return true;
        }
    }
}

bool parse_path(Parser *parser, AstPath *path) {
	if (!parse_decl_path(parser, &path->decl_path)) {
		return false;
	}
    path->inner_path.segments = vec_new(AstInnerPathSegment);
	if (parser_next_is_not(parser, TOKEN_DOT)) {
		return true;
	}
	return parse_inner_path(parser, &path->inner_path);
}
