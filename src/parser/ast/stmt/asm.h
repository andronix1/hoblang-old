#pragma once

#include "ast/node/stmt/asm.h"
#include "parser/interface/parser.h"

AstInlineAsm *parse_inline_asm(Parser *parser);
