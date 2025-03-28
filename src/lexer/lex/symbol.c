#include "lexer/lex/symbol.h"
#include "lexer/api.h"
#include "lexer/token/token.h"
#include "lexer/token/kind.h"
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
				lexer_token(lexer)->kind = alt->kind;
				return LEX_PART_OK;
			}
		}
		lexer_token(lexer)->kind = start->kind;
		return LEX_PART_OK;
	}
	return LEX_PART_MISSMATCH;
}

LexPartErr lex_symbol(Lexer *lexer, char symbol, TokenKind kind) {
	if (symbol == lexer_next_char(lexer)) {
		lexer_token(lexer)->kind = kind;
		return LEX_PART_OK;
	}
	return LEX_PART_MISSMATCH;
}
