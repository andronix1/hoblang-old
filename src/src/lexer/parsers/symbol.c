#include "../parsers.h"

LexOneErr lex_symbol_alt(Lexer *lexer, LexSymbolAlt *symbols, size_t len) {
	assert(len, "symbols len must be non-zero");
	LexSymbolAlt *start = &symbols[0];
	if (start->symbol == lexer_next_char(lexer)) {
		char futc = lexer_future_char(lexer);
		for (size_t i = 1; i < len; i++) {
			LexSymbolAlt *alt = &symbols[i];
			if (futc == alt->symbol) {
				lexer_next_char(lexer);
				lexer->token.type = alt->type;
				return LEX_ONE_OK;
			}
		}
		lexer->token.type = start->type;
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
