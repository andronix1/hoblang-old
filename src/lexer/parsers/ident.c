#include "symbol.h"

LexOneErr lex_ident(Lexer *lexer) {
	if (!char_is_ident_start(lexer_next_char(lexer))) {
		return LEX_ONE_MISSMATCH;
	}
	const char *ptr = lexer_str(lexer);
	size_t len = 1;	
	while (char_is_ident(lexer_future_char(lexer))) {
		lexer_next_char(lexer);
		len++;
	}
	lexer->token.type = TOKEN_IDENT;
	lexer->token.ident.str = ptr;
	lexer->token.ident.len = len;
	return LEX_ONE_OK;
}
