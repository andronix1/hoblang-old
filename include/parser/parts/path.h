#pragma once

#include <stdbool.h>
#include "ast/api/path.h"
#include "parser/parser.h"

bool parse_path(Parser *parser, AstPath *path);
bool parse_decl_path(Parser *parser, AstDeclPath *path);
bool parse_inner_path(Parser *parser, AstInnerPath *path);
