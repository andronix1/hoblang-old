#pragma once

#include "ast/node/stmt/defer.h"
#include "parser/interface/parser.h"

AstDefer *parse_defer(Parser *parser);
