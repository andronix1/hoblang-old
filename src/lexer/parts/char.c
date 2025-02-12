#include "lexer/private.h"
#include "lexer/part.h"

LexPartErr lex_char(Lexer *lexer) {
	if (lexer_next_char(lexer) != '\'') {
		return LEX_PART_MISSMATCH;
	}
	char nextc;
	if (!lexer_next_escaped(lexer, '\'', &nextc)) {
		return LEX_PART_ERR;
	}
	if (nextc != '\'') {
		lex_err("expected break");
	}
	lexer_token(lexer)->type = TOKEN_CHAR;
	return LEX_PART_OK;
}
