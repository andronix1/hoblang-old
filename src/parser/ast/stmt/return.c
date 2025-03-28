#include "return.h"
#include "ast/node/stmt/return.h"
#include "core/not_null.h"
#include "lexer/token/kind.h"
#include "parser/api.h"
#include "parser/ast/shared/expr.h"

AstReturn *parse_return(Parser *parser) {
    if (parser_next_is(parser, TOKEN_SEMICOLON)) {
        return ast_return_new(NULL);
    }
    AstReturn *result = ast_return_new(NOT_NULL(parse_expr(parser)));
    parser_expect_next(parser, TOKEN_SEMICOLON);
    return result;
}
