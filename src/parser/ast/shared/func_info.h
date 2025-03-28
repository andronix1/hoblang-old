#pragma once

#include "ast/interface/func_info.h"
#include "ast/shared/generic.h"
#include "parser/interface/parser.h"

AstFuncInfo *parse_func_info(Parser *parser, AstGenerics **generics_output);

