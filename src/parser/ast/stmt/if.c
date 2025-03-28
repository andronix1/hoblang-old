#include "ast/node/stmt/if.h"
#include "ast/interface/body.h"
#include "core/not_null.h"
#include "parser/api.h"
#include "core/vec.h"
#include "parser/ast/shared/body.h"
#include "parser/ast/shared/expr.h"

AstIfCondBlock *parse_if_cond_block(Parser *parser) {
    AstExpr *cond = NOT_NULL(parse_expr(parser));
    AstBody *body = NOT_NULL(parse_body(parser));
    return ast_if_cond_block_new(cond, body);
}

AstIf *parse_if(Parser *parser) {
    AstIfCondBlock *if_block = NOT_NULL(parse_if_cond_block(parser));
    AstBody *else_body = NULL;
    AstIfCondBlock **else_ifs = vec_new(AstIfCondBlock*);
    while (parser_next_is(parser, TOKEN_ELSE)) {
        if (parser_next_is(parser, TOKEN_IF)) {
            else_ifs = vec_push_dir(else_ifs, NOT_NULL(parse_if_cond_block(parser)));
        } else {
            else_body = NOT_NULL(parse_body(parser));
            break;
        }
    }
    return ast_if_new(if_block, else_ifs, else_body);
}
