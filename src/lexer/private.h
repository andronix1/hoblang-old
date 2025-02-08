#pragma once

#include "api.h"
#include "core/slice.h"

const char *lexer_str(const Lexer *lexer);
char lexer_next_char(Lexer *lexer);
bool lexer_next_escaped(Lexer *lexer, char stop, char *output);
char lexer_future_char(const Lexer *lexer);
void lexer_skip_whitespace(Lexer *lexer);
void lexer_fail(Lexer *lexer);

#define lex_log(level, fmt, ...) \
    do { \
        LexerPosition position = lexer_position(lexer); \
        hob_log_at((level), position.file, position.location, fmt, ##__VA_ARGS__); \
    } while (0)

#define lex_err(fmt, ...) \
	do { \
		lexer_fail(lexer); \
		lex_log(LOGE, fmt, #__VA_ARGS__); \
	} while (0)
