#pragma once

#include "ast/api/expr.h"
#include "parser/parser.h"
#include "lexer/token.h"

AstExpr *parse_expr(Parser *parser, bool (*stop)(TokenType));
