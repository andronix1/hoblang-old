#include "while.h"
#include "ast/interface/body.h"
#include "ast/node/stmt/while.h"
#include "core/not_null.h"
#include "lexer/token/kind.h"
#include "parser/api.h"
#include "parser/ast/shared/body.h"
#include "parser/ast/shared/expr.h"
#include "core/slice/api.h"

AstWhile *parse_while(Parser *parser) {
    bool is_named = parser_next_is(parser, TOKEN_OPENING_SQUARE_BRACE);
    Slice name = slice_from_const_cstr("");
    if (is_named) {
        name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "loop name")->ident;
        PARSER_EXPECT_NEXT(TOKEN_CLOSING_SQUARE_BRACE, "`]`");
    }
    AstExpr *cond = NOT_NULL(parse_expr(parser));
    AstBody *body = NOT_NULL(parse_body(parser));
    return is_named ?
        ast_while_new_named(cond, body, name) :
        ast_while_new(cond, body);
}
