#pragma once

#include <stdbool.h>
#include "ast/api/type.h"
#include "ast/api/func_info.h"
#include "parser/parser.h"
#include "ast/api/func_info.h"

bool parse_func_type(Parser *parser, AstFuncArg **args, AstType *returning);
bool parse_func_info(Parser *parser, AstFuncInfo *info);
