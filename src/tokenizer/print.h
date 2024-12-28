#pragma once

#include <stdio.h>
#include <stddef.h>
#include "chars.h"
#include "base.h"



void tokenizer_print_line(Tokenizer *tokenizer);
void tokenizer_print_column_ptr(Tokenizer *tokenizer);
void tokenizer_print_line_at(Tokenizer *tokenizer);
