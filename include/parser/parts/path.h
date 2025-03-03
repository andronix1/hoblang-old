#pragma once

#include <stdbool.h>
#include "ast/api/path.h"
#include "parser/parser.h"

bool parse_path(Parser *parser, AstPath *path);
