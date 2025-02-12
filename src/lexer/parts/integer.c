#include "lexer/private.h"
#include "lexer/part.h"
#include "core/chars.h"

int char_to_digit10(char c) { return c - '0'; }

int char_to_digit16(char c) {
	if (char_is_digit(c)) return char_to_digit10(c);
	if ('a' <= c && c <= 'f') return c - 'a' + 10;
	if ('A' <= c && c <= 'F') return c - 'A' + 10;
	assert(0, "invalid character '{int}' passed", c);
}

LexPartErr lex_integer(Lexer *lexer) {
	char c;
	if (!char_is_digit(c = lexer_next_char(lexer))) {
		return LEX_PART_MISSMATCH;
	}
	int base = 10;
	int(*convert)(char c) = char_to_digit10;
	bool(*is_right)(char c) = char_is_digit;
	if (c == '0' && lexer_future_char(lexer) == 'x') {
		base = 16;
		convert = char_to_digit16;
		is_right = char_is_hex;
		lexer_next_char(lexer);
		if (!is_right(c = lexer_next_char(lexer))) {
			lex_err("expected digit after 16 bit value prefix");
			return LEX_PART_ERR;
		}
	}
	uint64_t result = convert(c);
	while (is_right(c = lexer_future_char(lexer))) {
		lexer_next_char(lexer);
		result = result * base + convert(c);
	}
	Token *token = lexer_token(lexer);
	token->type = TOKEN_INTEGER;
	token->integer = result;
	return LEX_PART_OK;
}
