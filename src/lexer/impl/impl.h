#pragma once

#include "../private.h"
#include "../tokens.h"

typedef struct Lexer {
	const char *file;
    Slice full, remain;
    FileLocation location, start_location;
	size_t line_offset, delta;
    Token token;
	bool failed;
} Lexer;