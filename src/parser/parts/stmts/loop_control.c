#include "lexer/token.h"
#include "parser/private.h"
#include "ast/private/stmts/loop_control.h"

bool parse_loop_control(Parser *parser, AstStmtLoopControl *loop_control) {
    Token *token = parser_next_is_or(parser, TOKEN_IDENT);
    loop_control->is_named = token != NULL;
    if (token) {
        loop_control->loop_name = token->ident;
    }
    PARSER_EXPECT_NEXT(TOKEN_SEMICOLON, "semicolon");
    return true;
}
