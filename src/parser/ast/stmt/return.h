#pragma once

#include "ast/node/stmt/return.h"
#include "parser/interface/parser.h"

AstReturn *parse_return(Parser *parser);
