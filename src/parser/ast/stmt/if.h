#pragma once

#include "ast/node/stmt/if.h"
#include "parser/interface/parser.h"

AstIf *parse_if(Parser *parser);

