#pragma once

#include "core/vec.h"
#include "tokens.h"
#include "chars.h"
#include "base.h"
#include "print.h"

typedef Vec Tokens;

#define tokens_new() vec_new(Token)

typedef enum {
	TOKENIZE_OK = 0,
	TOKENIZE_ERR,
	TOKENIZE_FINISHED
} TokenizeErr;

TokenizeErr tokenizer_next(Tokenizer *tokenizer);
bool tokenize_all(Tokenizer *tokenizer, Tokens *to);
