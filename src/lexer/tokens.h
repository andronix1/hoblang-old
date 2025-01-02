#pragma once

#include <stdbool.h>
#include <printf.h>
#include <stdint.h>
#include "core/fatptr.h"
#include "core/location.h"

typedef enum { 
	TOKEN_FUN,
    TOKEN_VAR,
    TOKEN_RETURN,
    
	TOKEN_IF,
    TOKEN_ELSE,

    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_ASSIGN,

	TOKEN_OPENING_CIRCLE_BRACE,
	TOKEN_CLOSING_CIRCLE_BRACE,
	TOKEN_OPENING_FIGURE_BRACE,
	TOKEN_CLOSING_FIGURE_BRACE,
    
	TOKEN_ADD,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,

    TOKEN_EQUALS,
    TOKEN_NOT_EQUALS,
    TOKEN_LESS,
    TOKEN_LESS_OR_EQUALS,
    TOKEN_GREATER,
    TOKEN_GREATER_OR_EQUALS,

    TOKEN_INTEGER,
    TOKEN_IDENT,
	
	TOKEN_EOI 
} TokenType;

typedef struct {
    TokenType type;
    FileLocation location;
    union {
        FatPtr ident;
        uint64_t integer;
    };
} Token;

typedef struct {
	const Token *tokens;
	size_t len;
} TokensSlice;

TokenType token_type(Token *token);
void token_register_printf();
