#pragma once

#include "lexer/lexer.h"

bool lexer_next_escaped(Lexer *lexer, char stop, char *output);
LexOneErr lex_char(Lexer *lexer);
