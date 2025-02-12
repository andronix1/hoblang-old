#pragma once

#include <stdbool.h>
#include "ast/api/type.h"
#include "parser/parser.h"

bool parse_type(Parser *parser, AstType *type);
