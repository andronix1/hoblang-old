#pragma once

#include <stdbool.h>
#include "core/location.h"
#include "core/slice/slice.h"
#include "lexer/token/token.h"

typedef struct Lexer {
	const char *file;
    Slice full, remain;
    FileLocation location, start_location;
	size_t line_offset, delta;
    Token token;
	bool failed;
} Lexer;

