#pragma once

#include <stdbool.h>
#include "ast/api/func_info.h"
#include "parser/parser.h"
#include "ast/api/func_info.h"

bool parse_func_type_info(Parser *parser, AstFuncTypeInfo *info);
bool parse_func_decl_info(Parser *parser, AstFuncInfo *info);
bool parse_func_info(Parser *parser, AstFuncInfo *info);
