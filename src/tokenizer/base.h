#pragma once

#include <stdbool.h>
#include "core/fatptr.h"
#include "tokens.h"
#include "chars.h"

typedef struct {
    FatPtr remain;
    FileLocation location, start_location;
	size_t line_offset, delta;
    Token token;
} Tokenizer;

Tokenizer tokenizer_new(FatPtr fatptr);
char *tokenizer_str(Tokenizer *tokenizer);
bool tokenizer_finished(Tokenizer *tokenizer);
char tokenizer_next_char(Tokenizer *tokenizer);
char tokenizer_future_char(Tokenizer *tokenizer);
void tokenizer_begin(Tokenizer *tokenizer);
void tokenizer_rollback(Tokenizer *tokenizer);
void tokenizer_skip_whitespace(Tokenizer *tokenizer);
