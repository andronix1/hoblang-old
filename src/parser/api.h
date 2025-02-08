#pragma once

#include "lexer/api.h"

typedef struct Parser Parser;

Parser *parser_from_file(const char *path);
Parser *parser_new(Lexer *lexer);
bool parser_failed(const Parser *parser);