#pragma once

#include "lexer/api.h"

typedef struct Parser {
	Lexer *lexer;
	Token *token;
	bool skip_next, failed;
} Parser;