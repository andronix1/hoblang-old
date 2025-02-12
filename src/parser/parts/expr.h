#pragma once

#include "ast/api/expr.h"
#include "../parts.h"

AstExpr *parse_expr(Parser *parser, bool (*stop)(TokenType));
