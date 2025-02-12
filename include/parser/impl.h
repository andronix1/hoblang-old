#pragma once

#include "lexer/lexer.h"
#include "lexer/token.h"

typedef struct Parser {
	Lexer *lexer;
	Token *token;
	bool skip_next, failed;
} Parser;
