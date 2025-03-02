#pragma once

#include "ast/private/stmts/const.h"
#include "parser/parser.h"

bool parse_const(Parser *parser, AstConst *constant);
