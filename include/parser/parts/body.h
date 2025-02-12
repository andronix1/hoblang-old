#pragma once

#include <stdbool.h>
#include "ast/api/body.h"
#include "parser/parser.h"

bool parse_body(Parser *parser, AstBody *body);
bool parse_body_ola(Parser *parser, AstBody *body);
