#pragma once

#include "lexer/lexer.h"

typedef struct {
	Lexer *lexer;
	Token *token;
	bool skip_next, failed;
} Parser;

bool parser_init(Parser *parser, Lexer *lexer);
