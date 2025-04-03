#pragma once

#include "ast/interface/global.h"
#include "ast/node/decl.h"
#include "parser/interface/parser.h"

AstNodeDecl *parse_global_decl(Parser *parser);
