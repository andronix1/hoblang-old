#pragma once

#include "lexer/api.h"
#include "lexer/private.h"
#include "lexer/token.h"

typedef struct Lexer {
	const char *file;
    Slice full, remain;
    FileLocation location, start_location;
	size_t line_offset, delta;
    Token token;
	bool failed;
} Lexer;
