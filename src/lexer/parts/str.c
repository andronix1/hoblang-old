#include "lexer/private.h"
#include "lexer/part.h"
#include "core/chars.h"

LexPartErr lex_str(Lexer *lexer) {
	if (lexer_next_char(lexer) != '"') {
		return LEX_PART_MISSMATCH;
	}
	char *result = vec_new(char);
	char c;
	while ((c = lexer_future_char(lexer)) != '"' && c != (char)EOF) {
		if (!lexer_next_escaped(lexer, '"', &c)) {
			return LEX_PART_ERR;
		}
		result = vec_push(result, &c);
	}
	lexer_next_char(lexer);
	Token *token = lexer_token(lexer);
	token->type = TOKEN_STR;
	token->str = result;
	return LEX_PART_OK;
}
