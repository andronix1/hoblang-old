#include "../parsers.h"

bool lexer_next_escaped(Lexer *lexer, char stop, char *output) {
	char c;
	if ((c = lexer_next_char(lexer)) == (char)EOF) {
		lex_err("EOF while parsing character");
		return false;
	}
	if (c == '\\') {
		switch (c = lexer_next_char(lexer)){
			case '0':
				*output = '\0';
				return true;
			case 'r':
				*output = '\r';
				return true;
			case 'n':
				*output = '\n';
				return true;
			case (char)EOF:
				lex_err("EOF while parsing escape character");
				return false;
			default:
				if (c == stop) {
					*output = stop;
					return true;
				}
				lex_err("unknown escape symbol `%c`", c);
				return false;
		}
	} else if (c == stop) {
		lex_err("unexpected break while parsing character");
		return false;
	} else {
		*output = c;
		return true;
	}
}

LexOneErr lex_char(Lexer *lexer) {
	if (lexer_next_char(lexer) != '\'') {
		return LEX_ONE_MISSMATCH;
	}
	if (!lexer_next_escaped(lexer, '\'', &lexer->token.character)) {
		return LEX_ONE_ERR;
	}
	if (lexer_next_char(lexer) != '\'') {
		lex_err("expected break");
	}
	lexer->token.type = TOKEN_CHAR;
	return LEX_ONE_OK;
}
