#pragma once

#include "parser.h"
#include "lexer/lexer.h"
#include "ast/api/module.h"

Parser *parser_from_file(const char *path);
Parser *parser_new(Lexer *lexer);
bool parser_failed(const Parser *parser);
bool parse_module(Parser *parser, AstModule *module);
