#pragma once

#include <stdbool.h>
#include "core/location.h"
#include "lexer.h"
#include "token.h"

Lexer *lexer_from_file(const char *path);
Lexer *lexer_new(Slice content, const char *path);

Token *lexer_next(Lexer *lexer);
void lexer_begin(Lexer *lexer);
void lexer_rollback(Lexer *lexer);
bool lexer_finished(const Lexer *lexer);

bool lexer_failed(const Lexer *lexer);
InFilePosition lexer_position(const Lexer *lexer);

