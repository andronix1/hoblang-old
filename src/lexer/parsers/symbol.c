#include "../parsers.h"

LexOneErr lex_symbol_dual(Lexer *lexer, char symbol, char next, TokenType type, TokenType next_type) {
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

LexOneErr lex_symbol(Lexer *lexer, char symbol, TokenType type) {
	if (symbol == lexer_next_char(lexer)) {
		lexer->token.type = type;
		return LEX_ONE_OK;
	}
	return LEX_ONE_MISSMATCH;
}
