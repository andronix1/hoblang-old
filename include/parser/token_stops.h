#pragma once

#include <stdbool.h>
#include "lexer/token.h"

static inline bool token_stop_semicolon(TokenType type) { return type == TOKEN_SEMICOLON; }
static inline bool token_stop_closing_circle_brace(TokenType type) { return type == TOKEN_CLOSING_CIRCLE_BRACE; }
static inline bool token_stop_opening_figure_brace(TokenType type) { return type == TOKEN_OPENING_FIGURE_BRACE; }
static inline bool token_stop_funcall_arg(TokenType type) { return type == TOKEN_COMMA || type == TOKEN_CLOSING_CIRCLE_BRACE; }
static inline bool token_stop_array_arg(TokenType type) { return type == TOKEN_COMMA || type == TOKEN_CLOSING_FIGURE_BRACE; }
static inline bool token_stop_idx(TokenType type) { return type == TOKEN_CLOSING_SQUARE_BRACE; }

