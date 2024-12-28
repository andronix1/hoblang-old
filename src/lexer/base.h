#pragma once

#include "tokenizer/tokenizer.h"
#include "expr/parse.h"

typedef struct {
	Tokens *tokens;
} Lexer;

Lexer lexer_new(Tokens *tokens);

