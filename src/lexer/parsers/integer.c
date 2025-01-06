#include "integer.h"

LexOneErr lex_integer(Lexer *lexer) {
	char c;
	if (!char_is_digit(c = lexer_next_char(lexer))) {
		return LEX_ONE_MISSMATCH;
	}
	size_t result = c - '0';
	while (char_is_digit(c = lexer_future_char(lexer))) {
		lexer_next_char(lexer);
		result = result * 10 + c - '0';
	}
	lexer->token.type = TOKEN_INTEGER;
	lexer->token.integer = result;
	return LEX_ONE_OK;
}
