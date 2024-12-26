#pragma once

#include "core/ftx.h"
#include "tokens.h"

typedef struct {
    FileTxReader *reader;
    FileLocation location;
    Token token;
} Tokenizer;

Tokenizer tokenizer_new(FileTxReader *reader);
bool token_next(Tokenizer *tokenizer);
