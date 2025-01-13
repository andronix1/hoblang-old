#include "../parsers.h"
#include "core/vec.h"

LexOneErr lex_str(Lexer *lexer) {
	if (lexer_next_char(lexer) != '"') {
		return LEX_ONE_MISSMATCH;
	}
	char *result = vec_new(char);
	char c;
	while ((c = lexer_future_char(lexer)) != '"' && c != (char)EOF) {
		if (!lexer_next_escaped(lexer, '"', &c)) {
			return LEX_ONE_ERR;
		}
		result = vec_push(result, &c);
	}
	lexer_next_char(lexer);
	lexer->token.type = TOKEN_STR;
	lexer->token.str = result;
	return LEX_ONE_OK;
}
