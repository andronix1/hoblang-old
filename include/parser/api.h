#pragma once

#include <stdbool.h>
#include "parser.h"
#include "core/location.h"
#include "lexer/lexer.h"
#include "ast/api/module.h"

Parser *parser_from_file(const char *path);
Parser *parser_new(Lexer *lexer);
bool parser_failed(const Parser *parser);
bool parse_module(Parser *parser, AstModule *module);

void parser_print_line_error_at(Parser *parser, FileLocation at);
