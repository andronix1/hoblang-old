#pragma once

#include "lexer/token/kind.h"
#include "lexer/api.h"

typedef struct {
    char symbol;
    TokenKind kind;
} LexSymbolAlt;

LexPartErr lex_symbol_alt(Lexer *lexer, LexSymbolAlt *symbols, size_t len);
