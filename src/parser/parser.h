#pragma once

#include "base.h"
#include "ast.h"

#define parse_log(level, fmt, ...) hob_log_at(level, parser->lexer->file, parser->lexer->location, fmt, ##__VA_ARGS__)
#define parse_err(fmt, ...) \
	do { \
		parse_log(LOGE, fmt, ##__VA_ARGS__); \
		parser->failed = true; \
	} while (0)

bool parse_module(Parser *parser, AstModule *module);
