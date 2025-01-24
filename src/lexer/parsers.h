#pragma once

#include "lexer.h"
#include "core/assert.h"

typedef struct {
    char symbol;
    TokenType type;
} LexSymbolAlt;

LexOneErr lex_symbol_alt(Lexer *lexer, LexSymbolAlt *symbols, size_t len);
LexOneErr lex_symbol(Lexer *lexer, char symbol, TokenType type);
LexOneErr lex_keyword(Lexer *lexer, const char *keyword, TokenType type);
LexOneErr lex_integer(Lexer *lexer);
bool lexer_next_escaped(Lexer *lexer, char stop, char *output);
LexOneErr lex_char(Lexer *lexer);
LexOneErr lex_ident(Lexer *lexer);
LexOneErr lex_integer(Lexer *lexer);
LexOneErr lex_str(Lexer *lexer);
