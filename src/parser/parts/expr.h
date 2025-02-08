#pragma once

#include "../parts.h"

AstExpr *parse_expr(Parser *parser, bool (*stop)(TokenType));