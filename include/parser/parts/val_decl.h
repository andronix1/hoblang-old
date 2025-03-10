#pragma once

#include <stdbool.h>
#include "ast/api/type.h"
#include "ast/api/val_decl.h"
#include "parser/parser.h"

bool parse_val_decl(Parser *parser, AstValDecl *decl);


