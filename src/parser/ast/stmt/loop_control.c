#include "loop_control.h"
#include "ast/node/stmt/loop_control.h"
#include "lexer/token/kind.h"
#include "parser/api.h"

AstLoopControl *parse_loop_control(Parser *parser) {
    Token *label_token = parser_next_is_or(parser, TOKEN_IDENT);
    Slice label;
    if (label_token) {
        label = label_token->ident;
    }
    PARSER_EXPECT_NEXT(TOKEN_SEMICOLON, "semicolon");
    return label_token ?
        ast_loop_control_new_labelled(label) : 
        ast_loop_control_new();
}
