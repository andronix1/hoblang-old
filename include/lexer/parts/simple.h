#pragma once

#include "lexer/lexer.h"
#include "lexer/part.h"

LexPartErr lex_symbol(Lexer *lexer, char symbol, TokenType type);
LexPartErr lex_keyword(Lexer *lexer, const char *keyword, TokenType type);
LexPartErr lex_integer(Lexer *lexer);
LexPartErr lex_char(Lexer *lexer);
LexPartErr lex_ident(Lexer *lexer);
LexPartErr lex_integer(Lexer *lexer);
LexPartErr lex_str(Lexer *lexer);

