#pragma once

#include "lexer.h"
#include "parsers/char.h"
#include "parsers/ident.h"
#include "parsers/integer.h"
#include "parsers/keyword.h"
#include "parsers/symbol.h"

bool lex_next(Lexer *lexer);
