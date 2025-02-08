#pragma once

#include <stdbool.h>
#include "core/slice.h"
#include "tokens.h"

typedef struct Lexer Lexer;

typedef struct {
    const char *file;
    FileLocation location;
} LexerPosition;

bool lex_next(Lexer *lexer);
Lexer *lexer_from_file(const char *path);
Lexer *lexer_new(Slice content, const char *path);
void lexer_begin(Lexer *lexer);
void lexer_rollback(Lexer *lexer);
bool lexer_finished(Lexer *lexer);
bool lexer_failed(Lexer *lexer);
Token *lexer_token(Lexer *lexer);
LexerPosition lexer_position(Lexer *lexer);