#include "lexer/private.h"
#include "lexer/part.h"
#include "lexer/parts/symbol.h"
#include "core/chars.h"
#include "core/assert.h"

LexPartErr lex_symbol_alt(Lexer *lexer, LexSymbolAlt *symbols, size_t len) {
	assert(len, "symbols len must be non-zero");
	LexSymbolAlt *start = &symbols[0];
	if (start->symbol == lexer_next_char(lexer)) {
		char futc = lexer_future_char(lexer);
		for (size_t i = 1; i < len; i++) {
			LexSymbolAlt *alt = &symbols[i];
			if (futc == alt->symbol) {
				lexer_next_char(lexer);
				lexer_token(lexer)->type = alt->type;
				return LEX_PART_OK;
			}
		}
		lexer_token(lexer)->type = start->type;
		return LEX_PART_OK;
	}
	return LEX_PART_MISSMATCH;
}

LexPartErr lex_symbol(Lexer *lexer, char symbol, TokenType type) {
	if (symbol == lexer_next_char(lexer)) {
		lexer_token(lexer)->type = type;
		return LEX_PART_OK;
	}
	return LEX_PART_MISSMATCH;
}
