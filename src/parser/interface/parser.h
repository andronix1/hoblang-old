#pragma once

#include <stdbool.h>
#include "ast/interface/module.h"
#include "core/location.h"

typedef struct Parser Parser;

Parser *parser_from_file(const char *path);
AstModule *parse_module(Parser *parser);

bool parser_failed(const Parser *parser);
void parser_print_line_error_at(Parser *parser, FileLocation at);
