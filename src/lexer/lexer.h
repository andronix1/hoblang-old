#pragma once

#include <stdbool.h>
#include "tokens.h"
#include "core/chars.h"
#include "core/fatptr.h"
#include "core/log.h"
#include "tokens.h"

#define lex_log(level, fmt, ...) hob_log_at((level), lexer->file, lexer->location, fmt, ##__VA_ARGS__)
#define lex_err(fmt, ...) \
	do { \
		lexer->failed = true; \
		lex_log(LOGE, fmt, #__VA_ARGS__); \
	} while (0)

typedef struct {
	const char *file;
    FatPtr full, remain;
    FileLocation location, start_location;
	size_t line_offset, delta;
    Token token;
	bool failed;
} Lexer;

bool lexer_init(Lexer *lexer, const char *path);
char *lexer_str(Lexer *lexer);
bool lexer_finished(Lexer *lexer);
char lexer_next_char(Lexer *lexer);
char lexer_future_char(Lexer *lexer);
void lexer_begin(Lexer *lexer);
void lexer_rollback(Lexer *lexer);
void lexer_skip_whitespace(Lexer *lexer);
void lexer_free(Lexer *lexer);

typedef enum {
	LEX_ONE_OK = 0,
	LEX_ONE_ERR,
	LEX_ONE_MISSMATCH
} LexOneErr;
