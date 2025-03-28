#pragma once

#include "ast/interface/body.h"
#include "parser/interface/parser.h"

AstBody *parse_body(Parser *parser);
