#pragma once

#include "ast/interface/node.h"
#include "parser/interface/parser.h"

AstNodeDecl *parse_struct_decl(Parser *parser);
