#include "val.h"
#include "ast/interface/val_info.h"
#include "ast/node/decl.h"
#include "ast/node/decl/val.h"
#include "ast/shared/val_info.h"
#include "core/not_null.h"
#include "lexer/token/kind.h"
#include "parser/api.h"
#include "parser/ast/shared/expr.h"
#include "parser/ast/shared/val_info.h"

AstNodeDecl *parse_val_decl(Parser *parser) {
    AstValInfo *info = NOT_NULL(parse_val_info(parser));
    AstExpr *initializer = NULL;
    if (parser_next_is(parser, TOKEN_ASSIGN)) {
        initializer = parse_expr(parser);
    }
    PARSER_EXPECT_NEXT(TOKEN_SEMICOLON, ";");
    return ast_node_decl_new_val(info->name_loc, ast_val_decl_new(info, initializer));
}
