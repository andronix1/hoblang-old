#pragma once

#include "ast/interface/node.h"
#include "parser/interface/parser.h"

AstNodeDecl *parse_from_use(Parser *parser, FileLocation from_loc);
