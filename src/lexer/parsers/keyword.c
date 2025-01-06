#include "keyword.h"

LexOneErr lex_keyword(Lexer *lexer, const char *keyword, TokenType type) {
	for (size_t i = 0; keyword[i] != '\0'; i++) {
		if (lexer_next_char(lexer) != keyword[i]) {
			return LEX_ONE_MISSMATCH;
		}
	}
	lexer->token.type = type;
	if (char_is_ident(lexer_future_char(lexer))) {
		return LEX_ONE_MISSMATCH;
	}
	return LEX_ONE_OK;
}
