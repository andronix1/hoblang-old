#include "../private.h"
#include "impl.h"

inline Token *parser_token(const Parser *parser) {
    return parser->token;
}

inline void parser_step(Parser *parser) {
    static Token eoi = { .type = TOKEN_EOI };
	if (parser->skip_next) {
		parser->skip_next = false;
		return;
	}
	if (!lex_next(parser->lexer)) {
		parser->token = &eoi;
	} else {
		parser->token = lexer_token(parser->lexer);
	}
}

inline Token *parser_next(Parser *parser) {
    parser_step(parser);
    return parser_token(parser);
}

void parse_skip_to(Parser *parser, ParseStopFunc stop) {
	while (stop(parser_token(parser)->type) && parser_token(parser)->type != TOKEN_EOI) {
		parser_step(parser);
	}
}

inline void parser_fail(Parser *parser) {
    parser->failed = true;
}

inline void parser_skip_next(Parser *parser) {
    parser->skip_next = true;
}

InFilePosition parser_position(const Parser *parser) {
    return lexer_position(parser->lexer);
}

inline bool parser_next_is(Parser *parser, TokenType type) {
    if (parser_next(parser)->type == type) {
        return true;
    }
    parser_skip_next(parser);
    return false;
}

inline bool parser_next_is_not(Parser *parser, TokenType type) {
    if (parser_next(parser)->type != type) {
        parser_skip_next(parser);
        return true;
    }
    return false;
}

inline Token *parser_next_is_not_or(Parser *parser, TokenType type) {
    Token *token = parser_next(parser);
    if (token->type != type) {
        parser_skip_next(parser);
        return token;
    }
    return NULL;
}

inline Token *parser_expect_next(Parser *parser, TokenType type) {
    if (parser_next_is(parser, type)) {
        return parser_token(parser);
    }
    return NULL;
}
