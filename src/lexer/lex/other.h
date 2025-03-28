#pragma once

#include "lexer/token/kind.h"
#include "lexer/api.h"

LexPartErr lex_symbol(Lexer *lexer, char symbol, TokenKind kind);
LexPartErr lex_keyword(Lexer *lexer, const char *keyword, TokenKind kind);
LexPartErr lex_integer(Lexer *lexer);
LexPartErr lex_char(Lexer *lexer);
LexPartErr lex_ident(Lexer *lexer);
LexPartErr lex_integer(Lexer *lexer);
LexPartErr lex_str(Lexer *lexer);


