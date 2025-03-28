#pragma once

#include <stdint.h>
#include "core/location.h"
#include "core/slice/slice.h"
#include "lexer/token/kind.h"

typedef struct Token {
    TokenKind kind;
    FileLocation location;
    union {
        Slice ident;
        uint64_t integer;
        long double float_value;
		char character;
        char *str;
    };
} Token;
