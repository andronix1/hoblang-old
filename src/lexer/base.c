#include "base.h"

Lexer lexer_new(Tokens *tokens, const char *path) {
	Lexer result = {
		.path = path,
		.tokens = tokens,
		.state = {
			.location = { 0, 0 },
			.token = 0,
		}
	};
	return result;
}

size_t lexer_find_scoped(Lexer *lexer, TokenType type) {
	size_t len = 0;
	size_t level = 0; // TODO: stack
	while (true) {
		LexerState state = lexer->state;
		Token *token = lexer_next_token(lexer);
		if (!token) {
			lex_error("unexpected EOF");
			return 0;
		}
		if (token_type(token) == type) {
			if (level != 0) {
				len++;
				continue;
			}
			lexer->state = state;
			return len;
		}
		switch (token_type(token)) {
			case TOKEN_OPENING_CIRCLE_BRACE: case TOKEN_OPENING_FIGURE_BRACE:
				len++;
				level++;
				break;
			case TOKEN_CLOSING_FIGURE_BRACE: case TOKEN_CLOSING_CIRCLE_BRACE:
				if (len == 0) {
					lex_error("unexpected closing");
					return 0;
				}
				level--;
				len++;
				break;
			default:
				len++;
				break;
		}
	}
}

Token *lexer_future_token(Lexer *lexer) {
	if (lexer->tokens->len <= lexer->state.token + 1) {
		return NULL;
	}
	Token *token = vec_at(lexer->tokens, lexer->state.token);
	lexer->state.location = token->location;
	return token;
}

Token *lexer_next_token(Lexer *lexer) {
	Token *token = lexer_future_token(lexer);
	if (token) {
		lexer->state.token++;
	}
	return token;
}
