#include "tokenizer.h"

#define TOKENIZER_NEXT_CHAR tokenizer->reader->value

Tokenizer tokenizer_new(FileTxReader *reader) {
    Tokenizer result = {
        .reader = reader,
    };
    return result;
}

#define TOKENIZER_SET_ONECHAR(token_type) \
        tokenizer->token.type = token_type; \
        return true; \

#define CASE_ONECHAR(c, token_type) case c: { TOKENIZER_SET_ONECHAR(token_type) }

bool token_parse_onechar(Tokenizer *tokenizer) {
    switch (TOKENIZER_NEXT_CHAR) {
        CASE_ONECHAR(':', TOKEN_COLON)
        CASE_ONECHAR(',', TOKEN_COMMA)
        CASE_ONECHAR(';', TOKEN_SEMICOLON)
    }
    return false;
}

#define CASE_BRACE(c, brace_type, _opening) \
    case c: { \
        tokenizer->token.type = TOKEN_BRACE; \
        tokenizer->token.brace.type = brace_type; \
        tokenizer->token.brace.opening = _opening; \
        return true; \
    }

#define CASE_BRACES(opening_c, closing_c, brace_type) \
    CASE_BRACE(opening_c, brace_type, true) \
    CASE_BRACE(closing_c, brace_type, false)

bool token_parse_brace(Tokenizer *tokenizer) {
    switch (TOKENIZER_NEXT_CHAR) {
        CASE_BRACES('(', ')', TOKEN_BRACE_CIRCLE)
        CASE_BRACES('{', '}', TOKEN_BRACE_FIGURE)
        CASE_BRACES('[', ']', TOKEN_BRACE_SQUARE)
    }
    return false;
}

#define TOKENIZER_SET_BINOP(_binop_type) \
    tokenizer->token.type = TOKEN_BINOP; \
    tokenizer->token.binop_type = _binop_type; \
    return true;

#define CASE_BINOP(c, _binop_type) case c: { TOKENIZER_SET_BINOP(_binop_type) }
#define CASE_BINOP_WITH_NEXT_TYPED(c, nc, set_type, set_next_type) \
    case c: { \
		ftx_commit(tokenizer->reader); \
        if (!ftx_next(tokenizer->reader)) { \
            set_type \
        } \
        char value = TOKENIZER_NEXT_CHAR; \
        if (value == nc) { \
            set_next_type; \
        } \
		ftx_rollback(tokenizer->reader); \
        set_type \
    }
#define CASE_TOKEN_WITH_NEXT_BINOP(c, nc, _token_type, _next_binop_type) CASE_BINOP_WITH_NEXT_TYPED(c, nc, TOKENIZER_SET_ONECHAR(_token_type), TOKENIZER_SET_BINOP(_next_binop_type))
#define CASE_BINOP_WITH_NEXT(c, nc, _binop_type, _next_binop_type) CASE_BINOP_WITH_NEXT_TYPED(c, nc, TOKENIZER_SET_BINOP(_binop_type), TOKENIZER_SET_BINOP(_next_binop_type))

bool token_parse_binop(Tokenizer *tokenizer) {
    switch (tokenizer->reader->value) {
        CASE_BINOP('+', TOKEN_BINOP_ADD)
        CASE_BINOP('-', TOKEN_BINOP_MINUS)
        CASE_BINOP('*', TOKEN_BINOP_MULTIPLY)
        CASE_BINOP('/', TOKEN_BINOP_DIVIDE)
        CASE_BINOP_WITH_NEXT('>', '=', TOKEN_BINOP_GREATER, TOKEN_BINOP_GREATER_OR_EQUALS)
        CASE_BINOP_WITH_NEXT('<', '=', TOKEN_BINOP_LESS, TOKEN_BINOP_LESS_OR_EQUALS)
        CASE_TOKEN_WITH_NEXT_BINOP('=', '=', TOKEN_ASSIGN, TOKEN_BINOP_LESS_OR_EQUALS)
    }
    return false;
}

bool token_parse_keyword(Tokenizer *tokenizer, const char *keyword, TokenType type) {
	printf("NEXT\n");
    for (size_t i = 0; keyword[i] != '\0'; i++) {
		printf("'%c' vs '%c'\n", TOKENIZER_NEXT_CHAR, keyword[i]);
		if (i != 0 && !ftx_next(tokenizer->reader)) {
            ftx_rollback(tokenizer->reader);
            return false;
		}
        if (TOKENIZER_NEXT_CHAR != keyword[i]) {
            ftx_rollback(tokenizer->reader);
            return false;
        }
    }
    tokenizer->token.type = type;
    return true;
}

bool is_not_whitespace(char c) {
    return c != '\n' && c != ' ' && c != '\t';
}

bool token_next(Tokenizer *tokenizer) {
    if (!ftx_first(tokenizer->reader, is_not_whitespace)) {
        return false;
    }
	ftx_commit(tokenizer->reader);
    if (
        token_parse_onechar(tokenizer) || 
        token_parse_brace(tokenizer) || 
        token_parse_binop(tokenizer) ||
        token_parse_keyword(tokenizer, "fun", TOKEN_FUN) || 
        token_parse_keyword(tokenizer, "let", TOKEN_VAR)
    ) {
		ftx_commit(tokenizer->reader);
        return true;
    }
    printf("unknown token!\n");
    ftx_rollback(tokenizer->reader);
    return false;
}
