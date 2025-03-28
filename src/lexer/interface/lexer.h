#pragma once

#include <stdbool.h>
#include "core/location.h"
#include "lexer/interface/token.h"

typedef struct Lexer Lexer;

Lexer *lexer_new_from_file(const char *path);

Token *lexer_next(Lexer *lexer);
void lexer_begin(Lexer *lexer);
void lexer_rollback(Lexer *lexer);
bool lexer_finished(const Lexer *lexer);

bool lexer_failed(const Lexer *lexer);
InFilePosition lexer_position(const Lexer *lexer);

void lexer_print_line_error_at(Lexer *lexer, FileLocation at);
