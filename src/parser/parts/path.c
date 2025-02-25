#include "ast/private/path.h"
#include "lexer/token.h"
#include "parser/private.h"

bool parse_decl_path(Parser *parser, AstDeclPath *path) {
    path->segments = vec_new(AstDeclPathSegment);
	while (true) {
        Token *token = PARSER_EXPECT_NEXT(TOKEN_IDENT, "module decl name");
        AstDeclPathSegment segment = {
            .ident = token->ident,
            .loc = token->location
        };
		path->segments = vec_push(
			path->segments,
            &segment
		);
		if (parser_next_is_not(parser, TOKEN_DOUBLE_COLON)) {
			return true;
		}
	}
}

bool parse_inner_path(Parser *parser, AstInnerPath *path) {
    path->segments = vec_new(AstInnerPathSegment);
    bool first = true;
    while (true) {
		Token *token = parser_next(parser);
		AstInnerPathSegment segment;
        segment.loc = token->location;
		switch (token->type) {
			case TOKEN_NULL:
				segment.type = AST_INNER_PATH_SEG_NULL;
				break;
			case TOKEN_IDENT:
				segment.type = AST_INNER_PATH_SEG_IDENT;
				segment.ident = token->ident;
				break;
            case TOKEN_SIZEOF:
				segment.type = AST_INNER_PATH_SEG_SIZEOF;
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
