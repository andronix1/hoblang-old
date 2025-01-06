#pragma once

#include "lexer/lexer.h"

LexOneErr lex_symbol_dual(Lexer *lexer, char symbol, char next, TokenType type, TokenType next_type);
LexOneErr lex_symbol(Lexer *lexer, char symbol, TokenType type);
