#pragma once

#include "lexer/lexer.h"

LexOneErr lex_keyword(Lexer *lexer, const char *keyword, TokenType type);
