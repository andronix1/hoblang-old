#include "core/vec.h"
#include "print.h"
#include "tests.h"
#include "core/slice/api.h"
#include "lexer/interface/lexer.h"
#include "lexer/token/token.h"
#include <string.h>

#define ABS(v) (v < 0 ? -v : v)

bool tokens_eq(Token *a, Token *b) {
    if (a->kind != b->kind) {
        return false;
    }
    switch (a->kind) {
        case TOKEN_FUN:
        case TOKEN_DEFER:
        case TOKEN_TYPE:
        case TOKEN_IMPORT:
        case TOKEN_USE:
        case TOKEN_WHILE:
        case TOKEN_VAR:
        case TOKEN_RETURN:
        case TOKEN_EXTERN:
        case TOKEN_AS:
        case TOKEN_ASM:
        case TOKEN_DOLLAR:
        case TOKEN_VOLATILE:
        case TOKEN_CONST:
        case TOKEN_SIZEOF:
        case TOKEN_AUTO:
        case TOKEN_UNWRAP:
        case TOKEN_BREAK:
        case TOKEN_CONTINUE:
        case TOKEN_PUBLIC:
        case TOKEN_UNDEFINED:
        case TOKEN_FINAL:
        case TOKEN_FROM:
        case TOKEN_GLOBAL:
        case TOKEN_BEHAVIOUR:
        case TOKEN_STRUCT:
        case TOKEN_FUNC_RETURNS:
        case TOKEN_IF:
        case TOKEN_ELSE:
        case TOKEN_QUESTION_MARK:
        case TOKEN_NULL:
        case TOKEN_COLON:
        case TOKEN_COMMA:
        case TOKEN_SEMICOLON:
        case TOKEN_ASSIGN:
        case TOKEN_OPENING_CIRCLE_BRACE:
        case TOKEN_CLOSING_CIRCLE_BRACE:
        case TOKEN_OPENING_FIGURE_BRACE:
        case TOKEN_CLOSING_FIGURE_BRACE:
        case TOKEN_OPENING_SQUARE_BRACE:
        case TOKEN_CLOSING_SQUARE_BRACE:
        case TOKEN_ASSIGN_MOD:
        case TOKEN_ASSIGN_ADD:
        case TOKEN_ASSIGN_MINUS:
        case TOKEN_ASSIGN_MULTIPLY:
        case TOKEN_ASSIGN_DIVIDE:
        case TOKEN_MOD:
        case TOKEN_ADD:
        case TOKEN_MINUS:
        case TOKEN_MULTIPLY:
        case TOKEN_DIVIDE:
        case TOKEN_NOT:
        case TOKEN_DOT:
        case TOKEN_BITOR:
        case TOKEN_BITAND:
        case TOKEN_BITNOT:
        case TOKEN_XOR:
        case TOKEN_SHR:
        case TOKEN_SHL:
        case TOKEN_AND:
        case TOKEN_OR:
        case TOKEN_EQUALS:
        case TOKEN_NOT_EQUALS:
        case TOKEN_LESS:
        case TOKEN_LESS_OR_EQUALS:
        case TOKEN_GREATER:
        case TOKEN_GREATER_OR_EQUALS:
        case TOKEN_TRUE:
        case TOKEN_FALSE:
            return true;
        case TOKEN_CHAR: return a->character == b->character;
        case TOKEN_INTEGER: return a->integer == b->integer;
        case TOKEN_FLOAT: return ABS(a->float_value - b->float_value) < 1e-4;
        case TOKEN_IDENT: return slice_eq(&a->ident, &b->ident);
        case TOKEN_STR: return slice_eq(&a->str, &b->str);
        case TOKEN_EOI: return true;
    }
}

void run_test(const char *code, Token *token, size_t len, bool should_fail) {
    Lexer *lexer = lexer_new(slice_from_cstr(code), "<memory>");
    if (should_fail) {
        lexer_shut_up(lexer);
    }
    ASSERT(lexer, "failed to create an lexer");
    for (size_t i = 0; i < len; i++) {
        Token *next = lexer_next(lexer);
        if (!should_fail && lexer_failed(lexer)) {
            PANIC("lexer failed!");
        }
        ASSERT(tokens_eq(next, &token[i]), "token number %lu missmatch", i);
    }
    if (should_fail && !lexer_failed(lexer)) {
        PANIC("lexer must fail!");
    }
}

#define TEST(name, should_fail, code, ...) \
     do { \
         Token _tokens[] = { __VA_ARGS__ }; \
         run_test(code, _tokens, sizeof(_tokens) / sizeof(_tokens[0]), should_fail); \
         OK(name); \
     } while (0)

#define TOK(_kind) { .kind = TOKEN_##_kind }
#define TOK_IDENT(_str) { .kind = TOKEN_IDENT, .ident = slice_from_const_cstr(_str) }
#define TOK_STR(_str) { .kind = TOKEN_STR, .str = slice_from_const_cstr(_str) }
#define TOK_STR_DIR(_str, _len) { .kind = TOKEN_STR, .str = slice_new(_str, _len) }
#define TOK_INT(_int) { .kind = TOKEN_INTEGER, .integer = _int }
#define TOK_FLOAT(_float) { .kind = TOKEN_FLOAT, .float_value = _float }
#define TOK_CH(_ch) { .kind = TOKEN_CHAR, .character = _ch }

int main() {
    print_setup();

    TEST("simple tokens", false, "fun defer type import use while var return extern as asm $ volatile const sizeof _ unwrap break continue public undefined final from global behaviour struct -> if else ? null : , ; = ( ) { } [ ] %= += -= *= /= % + - * / ! . | & ~ ^ >> << && || == != < <= > >= true false", TOK(FUN), TOK(DEFER), TOK(TYPE), TOK(IMPORT), TOK(USE), TOK(WHILE), TOK(VAR), TOK(RETURN), TOK(EXTERN), TOK(AS), TOK(ASM), TOK(DOLLAR), TOK(VOLATILE), TOK(CONST), TOK(SIZEOF), TOK(AUTO), TOK(UNWRAP), TOK(BREAK), TOK(CONTINUE), TOK(PUBLIC), TOK(UNDEFINED), TOK(FINAL), TOK(FROM), TOK(GLOBAL), TOK(BEHAVIOUR), TOK(STRUCT), TOK(FUNC_RETURNS), TOK(IF), TOK(ELSE), TOK(QUESTION_MARK), TOK(NULL), TOK(COLON), TOK(COMMA), TOK(SEMICOLON), TOK(ASSIGN), TOK(OPENING_CIRCLE_BRACE), TOK(CLOSING_CIRCLE_BRACE), TOK(OPENING_FIGURE_BRACE), TOK(CLOSING_FIGURE_BRACE), TOK(OPENING_SQUARE_BRACE), TOK(CLOSING_SQUARE_BRACE), TOK(ASSIGN_MOD), TOK(ASSIGN_ADD), TOK(ASSIGN_MINUS), TOK(ASSIGN_MULTIPLY), TOK(ASSIGN_DIVIDE), TOK(MOD), TOK(ADD), TOK(MINUS), TOK(MULTIPLY), TOK(DIVIDE), TOK(NOT), TOK(DOT), TOK(BITOR), TOK(BITAND), TOK(BITNOT), TOK(XOR), TOK(SHR), TOK(SHL), TOK(AND), TOK(OR), TOK(EQUALS), TOK(NOT_EQUALS), TOK(LESS), TOK(LESS_OR_EQUALS), TOK(GREATER), TOK(GREATER_OR_EQUALS), TOK(TRUE), TOK(FALSE));
    TEST("characters", false, "'c' '\\0' '\\r' '\\n' '\\''", TOK_CH('c'), TOK_CH('\0'), TOK_CH('\r'), TOK_CH('\n'), TOK_CH('\''));
    TEST("integers", false, "0x0123032 1234", TOK_INT(0x0123032), TOK_INT(1234));
    TEST("floats", false, "1.123", TOK_FLOAT(1.123));
    TEST("idents", false, "a asd iff", TOK_IDENT("a"), TOK_IDENT("asd"), TOK_IDENT("iff"));
    TEST("string", false, "\"asd\\n\\r\" \"\\0\"", TOK_STR("asd\n\r"), TOK_STR_DIR("\0", 1));
    TEST("end of input", false, "", TOK(EOI));

    TEST("invalid token skipping", true, "123`098", TOK_INT(123), TOK_INT(98));
    return 0;
}
