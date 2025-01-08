#pragma once

#include "lexer/lex.h"

#define parse_log(level, fmt, ...) hob_log_at(level, parser->lexer->file, parser->lexer->location, fmt, ##__VA_ARGS__)
#define parse_err(fmt, ...) \
	do { \
		parse_log(LOGE, fmt, ##__VA_ARGS__); \
		parser->failed = true; \
	} while (0)
	
#define EXPECTED(what) "expected " what " found `%T`", parser->token

#define parse_exp(type, what) \
	do { \
		if (token_type(parser->token) != type) { \
			parse_err(EXPECTED(what)); \
			return false; \
		} \
	} while (0)

#define parse_exp_next(type, what) \
	do { \
		parser_next_token(parser); \
		parse_exp(type, what); \
	} while (0)

typedef struct {
	Lexer *lexer;
	Token *token;
	bool skip_next, failed;
} Parser;

typedef bool (*ParseStopFunc)(TokenType);

bool parser_init(Parser *parser, Lexer *lexer);
void parser_next_token(Parser *parser);
void parse_skip_to(Parser *parser, ParseStopFunc stop);
