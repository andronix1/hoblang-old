#pragma once

#include "ast/shared/generic.h"
#include "parser/interface/parser.h"

AstGenerics *parse_generics(Parser *parser);
AstGenerics *parse_generics_optional(Parser *parser);



