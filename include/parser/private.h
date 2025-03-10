#pragma once

#include "parser.h"
#include "api.h"
#include "lexer/token.h"
#include "core/log.h"

#define parse_log(level, fmt, ...) \
	do { \
		InFilePosition position = parser_position(parser); \
		hob_log_at(level, position.file, position.location, fmt, ##__VA_ARGS__); \
	} while (0)

#define PARSE_ERROR(fmt, ...) \
	do { \
		parse_log(LOGE, fmt, ##__VA_ARGS__); \
        parser_print_line_error_at(parser, parser_token(parser)->location); \
		parser_fail(parser); \
	} while (0)
	
#define EXPECTED(what) "expected " what " found `{tok}`", parser_token(parser)

#define PARSER_EXPECT_NEXT(type, what) \
    ({ \
        Token *_token = parser_expect_next(parser, type); \
        if (!_token) { \
            PARSE_ERROR(EXPECTED(what)); \
            return false; \
        } \
        _token; \
    })

typedef bool (*ParseStopFunc)(TokenType);

void parser_step(Parser *parser);
Token *parser_next(Parser *parser);
Token *parser_token(const Parser *parser);
void parser_skip_next(Parser *parser);
void parser_fail(Parser *parser);
InFilePosition parser_position(const Parser *parser);

Token *parser_expect_next(Parser *parser, TokenType type);
bool parser_next_is(Parser *parser, TokenType type);
bool parser_next_is_not(Parser *parser, TokenType type);
Token *parser_next_is_not_or(Parser *parser, TokenType type);
Token *parser_next_is_or(Parser *parser, TokenType type);

