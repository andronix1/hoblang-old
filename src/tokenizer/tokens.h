#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "core/fatptr.h"
#include "core/file.h"

typedef enum {
    TOKEN_FUN,
    TOKEN_VAR,

    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_ASSIGN,

    TOKEN_BRACE,
    TOKEN_BINOP,

    TOKEN_INTEGER,
    TOKEN_IDENT,
} TokenType;

typedef struct {
    FatPtr file;
    FileLocation location;
} TokenLocation;

typedef enum {
    TOKEN_BRACE_SQUARE,
    TOKEN_BRACE_CIRCLE,
    TOKEN_BRACE_FIGURE,
} TokenBraceType;

typedef struct {
    TokenBraceType type;
    bool opening;
} TokenBraceInfo;

typedef enum {
    TOKEN_BINOP_ADD,
    TOKEN_BINOP_MINUS,
    TOKEN_BINOP_MULTIPLY,
    TOKEN_BINOP_DIVIDE,

    TOKEN_BINOP_EQUALS,
    TOKEN_BINOP_NOT_EQUALS,
    TOKEN_BINOP_LESS,
    TOKEN_BINOP_LESS_OR_EQUALS,
    TOKEN_BINOP_GREATER,
    TOKEN_BINOP_GREATER_OR_EQUALS,
} TokenBinopType;

typedef struct {
    TokenType type;
    TokenLocation location;
    union {
        FatPtr ident;
        TokenBraceInfo brace;
        TokenBinopType binop_type;
        uint64_t integer;
    };
} Token;