#pragma once

#include "ast/node/stmt/while.h"
#include "parser/interface/parser.h"

AstWhile *parse_while(Parser *parser);
