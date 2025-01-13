#pragma once

#include "lexer.h"

LexOneErr lex_symbol_dual(Lexer *lexer, char symbol, char next, TokenType type, TokenType next_type);
LexOneErr lex_symbol(Lexer *lexer, char symbol, TokenType type);
LexOneErr lex_keyword(Lexer *lexer, const char *keyword, TokenType type);
LexOneErr lex_integer(Lexer *lexer);
bool lexer_next_escaped(Lexer *lexer, char stop, char *output);
LexOneErr lex_char(Lexer *lexer);
LexOneErr lex_ident(Lexer *lexer);
LexOneErr lex_integer(Lexer *lexer);
LexOneErr lex_str(Lexer *lexer);
