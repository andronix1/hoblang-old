#pragma once

#include "ast/api/body.h"
#include "../parts.h"

bool parse_body(Parser *parser, AstBody *body);
bool parse_body_ola(Parser *parser, AstBody *body);
