#include "lexer.h"

typedef enum {
	LEX_ONE_OK = 0,
	LEX_ONE_ERR,
	LEX_ONE_MISSMATCH
} LexOneErr;

LexOneErr lexer_parse_keyword(Lexer *lexer, const char *keyword, TokenType type) {
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

LexOneErr lexer_parse_symbol_with_next(Lexer *lexer, char symbol, char next, TokenType type, TokenType next_type) {
	if (symbol == lexer_next_char(lexer)) {
		if (lexer_future_char(lexer) == next) {
			lexer_next_char(lexer);
			lexer->token.type = next_type;
			return LEX_ONE_OK;
		}
		lexer->token.type = type;
		return LEX_ONE_OK;
	}
	return LEX_ONE_MISSMATCH;
}

LexOneErr lexer_parse_symbol(Lexer *lexer, char symbol, TokenType type) {
	if (symbol == lexer_next_char(lexer)) {
		lexer->token.type = type;
		return LEX_ONE_OK;
	}
	return LEX_ONE_MISSMATCH;
}

LexOneErr lexer_parse_ident(Lexer *lexer) {
	if (!char_is_ident_start(lexer_next_char(lexer))) {
		return LEX_ONE_MISSMATCH;
	}
	char *ptr = lexer_str(lexer);
	size_t len = 1;	
	while (char_is_ident(lexer_future_char(lexer))) {
		lexer_next_char(lexer);
		len++;
	}
	lexer->token.type = TOKEN_IDENT;
	lexer->token.ident.ptr = ptr;
	lexer->token.ident.size = len;
	return LEX_ONE_OK;
}

LexOneErr lexer_parse_integer(Lexer *lexer) {
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

#define TOKEN_PARSE(expr) \
	do { \
		LexOneErr err = (expr); \
		if (!err) return true; \
		lexer_rollback(lexer); \
		if (err == LEX_ONE_ERR) return true; \
	} while(0)

#define SYMBOL(c, type) TOKEN_PARSE(lexer_parse_symbol(lexer, c, type));
#define KEYWORD(s, type) TOKEN_PARSE(lexer_parse_keyword(lexer, s, type));
#define SYMBOL_DUAL(c1, c2, t1, t2) TOKEN_PARSE(lexer_parse_symbol_with_next(lexer, c1, c2, t1, t2));

bool lex_next(Lexer *lexer) {
	lexer_skip_whitespace(lexer);
	lexer_begin(lexer);
	if (lexer_finished(lexer)) {
		return false;
	}
	lexer->token.location = lexer->location;
	
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
	KEYWORD("var", TOKEN_VAR);
	KEYWORD("fun", TOKEN_FUN);
	KEYWORD("if", TOKEN_IF);
	KEYWORD("else", TOKEN_ELSE);
	KEYWORD("return", TOKEN_RETURN);
	TOKEN_PARSE(lexer_parse_ident(lexer));
	TOKEN_PARSE(lexer_parse_integer(lexer));

	lex_err("unknown token");
	return true;
}
