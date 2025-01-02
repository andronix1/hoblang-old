#pragma once

#include "tokens.h"
#include "chars.h"
#include "base.h"
#include "core/log.h"

#define lex_log(level, fmt, ...) hob_log_at((level), lexer->file, lexer->location, fmt, ##__VA_ARGS__)
#define lex_err(fmt, ...) \
	do { \
		lexer->failed = true; \
		lex_log(LOGE, fmt, #__VA_ARGS__); \
	} while (0)

bool lex_next(Lexer *lexer);
