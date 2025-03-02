#include "parser/private.h"
#include "parser/token_stops.h"
#include "parser/parts/expr.h"
#include "parser/parts/type.h"
#include "ast/private/stmts/const.h"

bool parse_const(Parser *parser, AstConst *constant) {
    constant->name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "name")->ident;
    PARSER_EXPECT_NEXT(TOKEN_COLON, "colon");
    if (!parse_type(parser, &constant->type)) {
        return false;
    }
    PARSER_EXPECT_NEXT(TOKEN_ASSIGN, "assign");
    constant->expr = parse_expr(parser, token_stop_semicolon);
    if (!constant->expr) {
        return false;
    }
    PARSER_EXPECT_NEXT(TOKEN_SEMICOLON, "semicolon");
    return true;
}
