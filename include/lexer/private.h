#pragma once

#include "core/log.h"
#include "api.h"
#include "lexer.h"
#include "token.h"

#define lex_log(level, fmt, ...) \
    do { \
        InFilePosition position = lexer_position(lexer); \
        hob_log_at((level), position.file, position.location, fmt, ##__VA_ARGS__); \
    } while (0)

#define lex_err(fmt, ...) \
	do { \
		lexer_fail(lexer); \
		lex_log(LOGE, fmt, #__VA_ARGS__); \
        InFilePosition position = lexer_position(lexer); \
        lexer_print_line_error_at(lexer, position.location); \
	} while (0)

Token *lexer_token(Lexer *lexer);
const char *lexer_str(const Lexer *lexer);
char lexer_next_char(Lexer *lexer);
bool lexer_next_escaped(Lexer *lexer, char stop, char *output);
char lexer_future_char(const Lexer *lexer);
void lexer_skip_whitespace(Lexer *lexer);
void lexer_fail(Lexer *lexer);

bool lexer_try_next(Lexer *lexer);
