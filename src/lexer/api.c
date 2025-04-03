#include "lexer/lexer.h"
#include "lexer/api.h"
#include "lexer/lex/symbol.h"
#include "lexer/lex/other.h"
#include "core/chars.h"
#include "lexer/token/token.h"

Token *lexer_token(Lexer *lexer) {
	return &lexer->token;
}

bool lexer_next_escaped(Lexer *lexer, char stop, char *output) {
	char c;
	if ((c = lexer_next_char(lexer)) == (char)EOF) {
		LEXER_ERROR("EOF while parsing character");
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
				LEXER_ERROR("EOF while parsing escape character");
				return false;
			default:
				if (c == stop) {
					*output = stop;
					return true;
				}
				LEXER_ERROR("unknown escape symbol `%c`", c);
				return false;
		}
	} else if (c == stop) {
		LEXER_ERROR("unexpected break while parsing character");
		return false;
	} else {
		*output = c;
		return true;
	}
}

const char *lexer_str(const Lexer *lexer) {
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

char lexer_future_char(const Lexer *lexer) {
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

#define TOKEN_PARSE(expr) \
	do { \
		LexPartErr err = (expr); \
		if (!err) return true; \
		lexer_rollback(lexer); \
		if (err == LEX_PART_ERR) return true; \
	} while(0)

#define SYMBOL(c, type) TOKEN_PARSE(lex_symbol(lexer, c, type));
#define KEYWORD(s, type) TOKEN_PARSE(lex_keyword(lexer, s, type));
#define SYMBOL_DUAL(c1, c2, t1, t2) do { \
		LexSymbolAlt alts[2] = { { .symbol = c1, .kind = t1 }, { .symbol = c2, .kind = t2 } }; \
		TOKEN_PARSE(lex_symbol_alt(lexer, alts, 2)); \
	} while (0)
#define SYMBOL_TRIAL(c1, c2, c3, t1, t2, t3) do { \
		LexSymbolAlt alts[3] = { { .symbol = c1, .kind = t1 }, { .symbol = c2, .kind = t2 } , { .symbol = c3, .kind = t3 } }; \
		TOKEN_PARSE(lex_symbol_alt(lexer, alts, 3)); \
	} while (0)

bool lexer_skip_comment(Lexer *lexer) {
	if (lexer_future_char(lexer) != '#') {
		return false;
	}
    lexer_next_char(lexer);
    if (lexer_future_char(lexer) == '`') {
	    while (lexer_next_char(lexer) != '`' || lexer_future_char(lexer) != '#') {
            if (lexer_finished(lexer)) {
                LEXER_ERROR("multiline comment started but was not finished");
                return true;
            }
        }
    } else {
	    while (lexer_next_char(lexer) != '\n' && !lexer_finished(lexer));
    }
	return true;
}

bool lexer_try_next(Lexer *lexer) {
	do {
		lexer_skip_whitespace(lexer);
	} while (lexer_skip_comment(lexer));
	lexer_begin(lexer);
	if (lexer_finished(lexer)) {
        lexer->token.kind = TOKEN_EOI;
		return true;
	}
	lexer->token.location = lexer->location;
	
	SYMBOL('{', TOKEN_OPENING_FIGURE_BRACE);
	SYMBOL('}', TOKEN_CLOSING_FIGURE_BRACE);
	SYMBOL('[', TOKEN_OPENING_SQUARE_BRACE);
	SYMBOL(']', TOKEN_CLOSING_SQUARE_BRACE);
	SYMBOL('(', TOKEN_OPENING_CIRCLE_BRACE);
	SYMBOL(')', TOKEN_CLOSING_CIRCLE_BRACE);
	SYMBOL_DUAL('+', '=', TOKEN_ADD, TOKEN_ASSIGN_ADD);
	SYMBOL_TRIAL('-', '=', '>', TOKEN_MINUS, TOKEN_ASSIGN_MINUS, TOKEN_FUNC_RETURNS);
	SYMBOL_DUAL('*', '=', TOKEN_MULTIPLY, TOKEN_ASSIGN_MULTIPLY);
	SYMBOL_DUAL('/', '=', TOKEN_DIVIDE, TOKEN_ASSIGN_DIVIDE);
	SYMBOL(';', TOKEN_SEMICOLON);
	SYMBOL(',', TOKEN_COMMA);
	SYMBOL('.', TOKEN_DOT);
	SYMBOL('~', TOKEN_BITNOT);
	SYMBOL('$', TOKEN_DOLLAR);
	SYMBOL_DUAL('%', '=', TOKEN_MOD, TOKEN_ASSIGN_MOD);
    SYMBOL('?', TOKEN_QUESTION_MARK);
	SYMBOL(':', TOKEN_COLON);
	SYMBOL_DUAL('|', '|', TOKEN_BITOR, TOKEN_OR);
	SYMBOL_DUAL('&', '&', TOKEN_BITAND, TOKEN_AND);
	SYMBOL_DUAL('=', '=', TOKEN_ASSIGN, TOKEN_EQUALS);
	SYMBOL_TRIAL('>', '>', '=', TOKEN_GREATER, TOKEN_SHR, TOKEN_GREATER_OR_EQUALS);
	SYMBOL_TRIAL('<', '<', '=', TOKEN_LESS, TOKEN_SHL, TOKEN_LESS_OR_EQUALS);
	SYMBOL_DUAL('!', '=', TOKEN_NOT, TOKEN_NOT_EQUALS);
	KEYWORD("asm", TOKEN_ASM);
	KEYWORD("_", TOKEN_AUTO);
	KEYWORD("unwrap", TOKEN_UNWRAP);
	KEYWORD("global", TOKEN_GLOBAL);
	KEYWORD("break", TOKEN_BREAK);
	KEYWORD("continue", TOKEN_CONTINUE);
	KEYWORD("public", TOKEN_PUBLIC);
	KEYWORD("undefined", TOKEN_UNDEFINED);
	KEYWORD("final", TOKEN_FINAL);
	KEYWORD("from", TOKEN_FROM);
	KEYWORD("behaviour", TOKEN_BEHAVIOUR);
	KEYWORD("null", TOKEN_NULL);
	KEYWORD("sizeof", TOKEN_SIZEOF);
	KEYWORD("const", TOKEN_CONST);
	KEYWORD("volatile", TOKEN_VOLATILE);
	KEYWORD("var", TOKEN_VAR);
	KEYWORD("asm", TOKEN_ASM);
	KEYWORD("struct", TOKEN_STRUCT);
	KEYWORD("defer", TOKEN_DEFER);
	KEYWORD("type", TOKEN_TYPE);
	KEYWORD("fun", TOKEN_FUN);
	KEYWORD("if", TOKEN_IF);
	KEYWORD("else", TOKEN_ELSE);
	KEYWORD("return", TOKEN_RETURN);
	KEYWORD("extern", TOKEN_EXTERN);
	KEYWORD("import", TOKEN_IMPORT);
	KEYWORD("use", TOKEN_USE);
	KEYWORD("as", TOKEN_AS);
	KEYWORD("true", TOKEN_TRUE);
	KEYWORD("false", TOKEN_FALSE);
	KEYWORD("while", TOKEN_WHILE);
	TOKEN_PARSE(lex_char(lexer));
	TOKEN_PARSE(lex_ident(lexer));
	TOKEN_PARSE(lex_integer(lexer));
	TOKEN_PARSE(lex_str(lexer));

	LEXER_ERROR("failed to recognize token");
	return false;
}


