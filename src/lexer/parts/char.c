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
	if (lexer_next_char(lexer) != '\'') {
		lex_err("expected break");
	}
	Token *token = lexer_token(lexer);
	token->type = TOKEN_CHAR;
	token->character = nextc;
	return LEX_PART_OK;
}
