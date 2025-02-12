#pragma once

#include <stdbool.h>
#include "parser/parser.h"
#include "ast/api/func_info.h"

bool parse_func_info(Parser *parser, AstFuncInfo *info);
