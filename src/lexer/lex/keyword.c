#include "lexer/api.h"
#include "lexer/token/token.h"
#include "core/chars.h"

LexPartErr lex_keyword(Lexer *lexer, const char *keyword, TokenKind kind) {
	for (size_t i = 0; keyword[i] != '\0'; i++) {
		if (lexer_next_char(lexer) != keyword[i]) {
			return LEX_PART_MISSMATCH;
		}
	}
	lexer_token(lexer)->kind = kind;
	if (char_is_ident(lexer_future_char(lexer))) {
		return LEX_PART_MISSMATCH;
	}
	return LEX_PART_OK;
}
