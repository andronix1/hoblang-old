#pragma once

#include <stdbool.h>
#include "ast/api/path.h"
#include "parser/parser.h"

AstPath *parse_path(Parser *parser);
