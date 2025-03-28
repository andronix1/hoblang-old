#pragma once

#include "ast/interface/expr.h"
#include "parser/interface/parser.h"

AstExpr *parse_expr(Parser *parser);

