#include "body.h"
#include "ast/interface/node.h"
#include "ast/shared/body.h"
#include "core/not_null.h"
#include "core/vec.h"
#include "parser/api.h"
#include "parser/ast/node.h"

AstBody *parse_body(Parser *parser) {
    AstNode **nodes = vec_new(AstNode*);
    PARSER_EXPECT_NEXT(TOKEN_OPENING_FIGURE_BRACE, "{");
    while (parser_next_is_not(parser, TOKEN_CLOSING_FIGURE_BRACE)) {
        nodes = vec_push_dir(nodes, NOT_NULL(parse_node(parser)));
    }
    return ast_body_new(nodes);
}
