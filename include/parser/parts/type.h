#pragma once

#include <stdbool.h>
#include "ast/api/type.h"
#include "parser/parser.h"

AstType *parse_type(Parser *parser);
