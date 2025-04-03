#pragma once

#include "ast/interface/global.h"
#include "parser/interface/parser.h"

AstGlobalDeclInfo *parse_global_decl_info(Parser *parser);
