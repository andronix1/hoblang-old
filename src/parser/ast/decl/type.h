#pragma once

#include "ast/interface/node.h"
#include "parser/interface/parser.h"

AstNodeDecl *parse_type_alias(Parser *parser);
