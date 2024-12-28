#include "tokenizer.h"

typedef enum {
	TOKEN_PARSE_OK = 0,
	TOKEN_PARSE_ERR,
	TOKEN_PARSE_MISSMATCH
} TokenParseErr;

TokenParseErr tokenizer_parse_keyword(Tokenizer *tokenizer, const char *keyword, TokenType type) {
	for (size_t i = 0; keyword[i] != '\0'; i++) {
		if (tokenizer_next_char(tokenizer) != keyword[i]) {
			return TOKEN_PARSE_MISSMATCH;
		}
	}
	tokenizer->token.type = type;
	if (char_is_ident(tokenizer_future_char(tokenizer))) {
		return TOKEN_PARSE_MISSMATCH;
	}
	return TOKEN_PARSE_OK;
}

TokenParseErr tokenizer_parse_symbol_with_next(Tokenizer *tokenizer, char symbol, char next, TokenType type, TokenType next_type) {
	if (symbol == tokenizer_next_char(tokenizer)) {
		if (tokenizer_future_char(tokenizer) == next) {
			tokenizer_next_char(tokenizer);
			tokenizer->token.type = next_type;
			return TOKEN_PARSE_OK;
		}
		tokenizer->token.type = type;
		return TOKEN_PARSE_OK;
	}
	return TOKEN_PARSE_MISSMATCH;
}

TokenParseErr tokenizer_parse_symbol(Tokenizer *tokenizer, char symbol, TokenType type) {
	if (symbol == tokenizer_next_char(tokenizer)) {
		tokenizer->token.type = type;
		return TOKEN_PARSE_OK;
	}
	return TOKEN_PARSE_MISSMATCH;
}

TokenParseErr tokenizer_parse_ident(Tokenizer *tokenizer) {
	if (!char_is_ident_start(tokenizer_next_char(tokenizer))) {
		return TOKEN_PARSE_MISSMATCH;
	}
	char *ptr = tokenizer_str(tokenizer);
	size_t len = 1;	
	while (char_is_ident(tokenizer_future_char(tokenizer))) {
		tokenizer_next_char(tokenizer);
		len++;
	}
	tokenizer->token.type = TOKEN_IDENT;
	tokenizer->token.ident.ptr = ptr;
	tokenizer->token.ident.size = len;
	return TOKEN_PARSE_OK;
}

TokenParseErr tokenizer_parse_integer(Tokenizer *tokenizer) {
	char c;
	if (!char_is_digit(c = tokenizer_next_char(tokenizer))) {
		return TOKEN_PARSE_MISSMATCH;
	}
	size_t result = c - '0';
	while (char_is_digit(c = tokenizer_next_char(tokenizer))) {
		result = result * 10 + c - '0';
	}
	tokenizer->token.type = TOKEN_INTEGER;
	tokenizer->token.integer = result;
	return TOKEN_PARSE_OK;
}

#define TOKEN_PARSE(expr) \
	do { \
		TokenParseErr err = (expr); \
		if (!err) return TOKENIZE_OK; \
		tokenizer_rollback(tokenizer); \
		if (err == TOKEN_PARSE_ERR) return TOKENIZE_ERR; \
	} while(0)

#define SYMBOL(c, type) TOKEN_PARSE(tokenizer_parse_symbol(tokenizer, c, type));
#define KEYWORD(s, type) TOKEN_PARSE(tokenizer_parse_keyword(tokenizer, s, type));
#define SYMBOL_DUAL(c1, c2, t1, t2) TOKEN_PARSE(tokenizer_parse_symbol_with_next(tokenizer, c1, c2, t1, t2));

TokenizeErr tokenizer_next(Tokenizer *tokenizer) {
	tokenizer_skip_whitespace(tokenizer);
	tokenizer_begin(tokenizer);
	if (tokenizer_finished(tokenizer)) {
		return TOKENIZE_FINISHED;
	}
	
	SYMBOL('{', TOKEN_OPENING_FIGURE_BRACE);
	SYMBOL('}', TOKEN_CLOSING_FIGURE_BRACE);
	SYMBOL('(', TOKEN_OPENING_CIRCLE_BRACE);
	SYMBOL(')', TOKEN_CLOSING_CIRCLE_BRACE);
	SYMBOL('+', TOKEN_ADD);
	SYMBOL('-', TOKEN_MINUS);
	SYMBOL('*', TOKEN_MULTIPLY);
	SYMBOL('/', TOKEN_DIVIDE);
	SYMBOL(':', TOKEN_COLON);
	SYMBOL(';', TOKEN_SEMICOLON);
	SYMBOL(',', TOKEN_COMMA);
	SYMBOL_DUAL('=', '=', TOKEN_ASSIGN, TOKEN_EQUALS);
	SYMBOL_DUAL('>', '=', TOKEN_GREATER, TOKEN_GREATER_OR_EQUALS);
	SYMBOL_DUAL('<', '=', TOKEN_LESS, TOKEN_LESS_OR_EQUALS);
	KEYWORD("let", TOKEN_LET);
	KEYWORD("fun", TOKEN_FUN);
	KEYWORD("if", TOKEN_IF);
	KEYWORD("else", TOKEN_ELSE);
	TOKEN_PARSE(tokenizer_parse_ident(tokenizer));
	TOKEN_PARSE(tokenizer_parse_integer(tokenizer));

	printf("error: unknown token!\n");
	tokenizer_print_line_at(tokenizer);
	return TOKENIZE_ERR;
}

bool tokenize_all(Tokenizer *tokenizer, Tokens *to) {
	assert(to->len == 0);
	TokenizeErr err;
	while ((err = tokenizer_next(tokenizer)) == TOKENIZE_OK) {
		vec_push(to, &tokenizer->token);
	}
	if (err == TOKENIZE_ERR) {
		vec_free(to);
	}
	return err == TOKENIZE_FINISHED;
}
