#pragma once

#include "tokenizer/tokenizer.h"
#include "expr/expr.h"

#define lex_error(fmt, ...) printf("%s:%ld:%ld - error: " fmt "\n", lexer->path, lexer->state.location.line, lexer->state.location.column, ##__VA_ARGS__)
#define lex_error_at(pos, fmt, ...) printf("%s:%ld:%ld - error: " fmt "\n", lexer->path, pos.line, pos.column, ##__VA_ARGS__)

typedef struct {
	FileLocation location;
	size_t token;
} LexerState;

typedef struct {
	const char *path;
	Tokens *tokens;
	LexerState state;
} Lexer;

Lexer lexer_new(Tokens *tokens, const char *path);
size_t lexer_find_scoped(Lexer *lexer, TokenType type);
Token *lexer_future_token(Lexer *lexer);
Token *lexer_next_token(Lexer *lexer);
Token *lexer_current_token(Lexer *lexer);
