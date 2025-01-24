#include "lex.h"

#define TOKEN_PARSE(expr) \
	do { \
		LexOneErr err = (expr); \
		if (!err) return true; \
		lexer_rollback(lexer); \
		if (err == LEX_ONE_ERR) return true; \
	} while(0)

#define SYMBOL(c, type) TOKEN_PARSE(lex_symbol(lexer, c, type));
#define KEYWORD(s, type) TOKEN_PARSE(lex_keyword(lexer, s, type));
#define SYMBOL_DUAL(c1, c2, t1, t2) do { \
		LexSymbolAlt alts[2] = { { .symbol = c1, .type = t1 }, { .symbol = c2, .type = t2 } }; \
		TOKEN_PARSE(lex_symbol_alt(lexer, alts, 2)); \
	} while (0)
#define SYMBOL_TRIAL(c1, c2, c3, t1, t2, t3) do { \
		LexSymbolAlt alts[3] = { { .symbol = c1, .type = t1 }, { .symbol = c2, .type = t2 } , { .symbol = c3, .type = t3 } }; \
		TOKEN_PARSE(lex_symbol_alt(lexer, alts, 3)); \
	} while (0)

bool lexer_skip_comment(Lexer *lexer) {
	if (lexer_future_char(lexer) != '#') {
		return false;
	}
	while (lexer_next_char(lexer) != '\n');
	return true;
}

bool lex_next(Lexer *lexer) {
	do {
		lexer_skip_whitespace(lexer);
	} while (lexer_skip_comment(lexer));
	lexer_begin(lexer);
	if (lexer_finished(lexer)) {
		return false;
	}
	lexer->token.location = lexer->location;
	
	SYMBOL('{', TOKEN_OPENING_FIGURE_BRACE);
	SYMBOL('}', TOKEN_CLOSING_FIGURE_BRACE);
	SYMBOL('[', TOKEN_OPENING_SQUARE_BRACE);
	SYMBOL(']', TOKEN_CLOSING_SQUARE_BRACE);
	SYMBOL('(', TOKEN_OPENING_CIRCLE_BRACE);
	SYMBOL(')', TOKEN_CLOSING_CIRCLE_BRACE);
	SYMBOL('+', TOKEN_ADD);
	SYMBOL('-', TOKEN_MINUS);
	SYMBOL('*', TOKEN_MULTIPLY);
	SYMBOL('/', TOKEN_DIVIDE);
	SYMBOL(';', TOKEN_SEMICOLON);
	SYMBOL(',', TOKEN_COMMA);
	SYMBOL('.', TOKEN_DOT);
	SYMBOL_DUAL(':', ':', TOKEN_COLON, TOKEN_DOUBLE_COLON);
	SYMBOL_DUAL('|', '|', TOKEN_BITOR, TOKEN_OR);
	SYMBOL_DUAL('&', '&', TOKEN_BITAND, TOKEN_AND);
	SYMBOL_DUAL('=', '=', TOKEN_ASSIGN, TOKEN_EQUALS);
	SYMBOL_TRIAL('>', '>', '=', TOKEN_GREATER, TOKEN_SHR, TOKEN_GREATER_OR_EQUALS);
	SYMBOL_TRIAL('<', '<', '=', TOKEN_LESS, TOKEN_SHL, TOKEN_LESS_OR_EQUALS);
	SYMBOL_DUAL('!', '=', TOKEN_NOT, TOKEN_NOT_EQUALS);
	KEYWORD("var", TOKEN_VAR);
	KEYWORD("struct", TOKEN_STRUCT);
	KEYWORD("defer", TOKEN_DEFER);
	KEYWORD("type", TOKEN_TYPE);
	KEYWORD("fun", TOKEN_FUN);
	KEYWORD("if", TOKEN_IF);
	KEYWORD("else", TOKEN_ELSE);
	KEYWORD("return", TOKEN_RETURN);
	KEYWORD("extern", TOKEN_EXTERN);
	KEYWORD("import", TOKEN_IMPORT);
	KEYWORD("use", TOKEN_USE);
	KEYWORD("as", TOKEN_AS);
	KEYWORD("true", TOKEN_TRUE);
	KEYWORD("false", TOKEN_FALSE);
	KEYWORD("while", TOKEN_WHILE);
	TOKEN_PARSE(lex_char(lexer));
	TOKEN_PARSE(lex_ident(lexer));
	TOKEN_PARSE(lex_integer(lexer));
	TOKEN_PARSE(lex_str(lexer));

	lex_err("unknown token");
	lexer_next_char(lexer);
	return true;
}
