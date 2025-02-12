#pragma once

#include "lexer/token.h"
#include "lexer/part.h"

typedef struct {
    char symbol;
    TokenType type;
} LexSymbolAlt;

LexPartErr lex_symbol_alt(Lexer *lexer, LexSymbolAlt *symbols, size_t len);
