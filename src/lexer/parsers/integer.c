#include "../parsers.h"

LexOneErr lex_integer(Lexer *lexer) {
	char c;
	if (!char_is_digit(c = lexer_next_char(lexer))) {
		return LEX_ONE_MISSMATCH;
	}
	int base = 10;
	if (c == '0' && lexer_future_char(lexer) == 'x') {
		base = 16;
		lexer_next_char(lexer);
		if (!char_is_digit(c = lexer_next_char(lexer))) {
			lex_err("expected digit after 16 bit value prefix");
			return LEX_ONE_ERR;
		}
	}
	uint64_t result = c - '0';
	while (char_is_digit(c = lexer_future_char(lexer))) {
		if (base == 16) printf("%c\n", c);
		lexer_next_char(lexer);
		result = result * base + c - '0';
	}
	lexer->token.type = TOKEN_INTEGER;
	lexer->token.integer = result;
	return LEX_ONE_OK;
}
