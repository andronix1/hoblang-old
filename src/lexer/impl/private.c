#include "impl.h"
#include "core/chars.h"

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

const char *lexer_str(Lexer *lexer) {
	return lexer->remain.str - 1;
}

char lexer_next_char(Lexer *lexer) {
	if (lexer_finished(lexer)) {
		return EOF;
	}
	lexer->delta++;
	lexer->remain.len--;
	char c = *(lexer->remain.str++);
	if (c == '\n') {
		lexer->line_offset = 0;
		lexer->location.line++;
		lexer->location.column = 0;
	} else {
		lexer->line_offset++;
		lexer->location.column++;
	}
	return c;
}

char lexer_future_char(Lexer *lexer) {
	if (lexer_finished(lexer)) {
		return EOF;
	}
	return *lexer->remain.str;
}


void lexer_skip_whitespace(Lexer *lexer) {
	while (char_is_whitespace(lexer_future_char(lexer))) {
		lexer_next_char(lexer);
	}
}

void lexer_fail(Lexer *lexer) {
    lexer->failed = true;
}