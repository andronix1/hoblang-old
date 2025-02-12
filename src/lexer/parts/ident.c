#include "lexer/private.h"
#include "lexer/part.h"
#include "core/chars.h"

LexPartErr lex_ident(Lexer *lexer) {
	if (!char_is_ident_start(lexer_next_char(lexer))) {
		return LEX_PART_MISSMATCH;
	}
	const char *ptr = lexer_str(lexer);
	size_t len = 1;	
	while (char_is_ident(lexer_future_char(lexer))) {
		lexer_next_char(lexer);
		len++;
	}
	Token *token = lexer_token(lexer);
	token->type = TOKEN_IDENT;
	token->ident.str = ptr;
	token->ident.len = len;
	return LEX_PART_OK;
}
