#pragma once

#include "part.h"
#include "tokens.h"
#include "parts/symbol.h"

LexPartErr lex_symbol_alt(Lexer *lexer, LexSymbolAlt *symbols, size_t len);
LexPartErr lex_symbol(Lexer *lexer, char symbol, TokenType type);
LexPartErr lex_keyword(Lexer *lexer, const char *keyword, TokenType type);
LexPartErr lex_integer(Lexer *lexer);
LexPartErr lex_char(Lexer *lexer);
LexPartErr lex_ident(Lexer *lexer);
LexPartErr lex_integer(Lexer *lexer);
LexPartErr lex_str(Lexer *lexer);
