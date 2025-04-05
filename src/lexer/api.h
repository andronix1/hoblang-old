#pragma once

#include "lexer/interface/lexer.h"
#include "lexer/interface/token.h"
#include "core/log.h"

typedef enum {
	LEX_PART_OK = 0,
	LEX_PART_ERR,
	LEX_PART_MISSMATCH
} LexPartErr;

Token *lexer_token(Lexer *lexer);
const char *lexer_str(const Lexer *lexer);
char lexer_next_char(Lexer *lexer);
bool lexer_next_escaped(Lexer *lexer, char stop, char *output);
char lexer_future_char(const Lexer *lexer);
void lexer_skip_whitespace(Lexer *lexer);
void lexer_fail(Lexer *lexer);
bool lexer_quiet(Lexer *lexer);

bool lexer_try_next(Lexer *lexer);

#define LEXER_LOG(level, fmt, ...) \
    do { \
        if (lexer_quiet(lexer)) break; \
        InFilePosition position = lexer_position(lexer); \
        hob_log_at((level), position.file, position.location, fmt, ##__VA_ARGS__); \
    } while (0)

#define LEXER_ERROR(fmt, ...) \
	do { \
		lexer_fail(lexer); \
        if (lexer_quiet(lexer)) break; \
		LEXER_LOG(LOGE, fmt, #__VA_ARGS__); \
        InFilePosition position = lexer_position(lexer); \
        lexer_print_line_error_at(lexer, position.location); \
	} while (0)

