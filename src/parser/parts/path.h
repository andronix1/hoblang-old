#pragma once

#include "ast/api/path.h"
#include "../parts.h"

bool parse_path(Parser *parser, AstPath *path);
bool parse_decl_path(Parser *parser, AstDeclPath *path);
bool parse_inner_path(Parser *parser, AstInnerPath *path);
