#include "global.h"
#include "ast/node/decl/func.h"
#include "ast/node/decl/global.h"
#include "ast/node/decl/val.h"
#include "ast/shared/val_info.h"
#include "ast/shared/func_info.h"
#include "ast/node/decl.h"
#include "core/not_null.h"
#include "lexer/token/kind.h"
#include "parser/api.h"
#include "parser/ast/shared/body.h"
#include "parser/ast/shared/expr.h"
#include "parser/ast/shared/func_info.h"
#include "parser/ast/shared/val_info.h"

AstNodeDecl *parse_global_decl(Parser *parser) {
    PARSER_EXPECT_NEXT(TOKEN_OPENING_CIRCLE_BRACE, "`(`");
    Slice public_name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "public name")->ident;
    PARSER_EXPECT_NEXT(TOKEN_CLOSING_CIRCLE_BRACE, "`)`");
    switch (parser_next(parser)->kind) {
        case TOKEN_FUN: {
            AstFuncInfo *func_info = NOT_NULL(parse_func_info(parser, NULL));
            AstBody *body = NOT_NULL(parse_body(parser));
            return ast_node_decl_new_global(ast_global_decl_new_func(public_name, ast_func_decl_new(func_info, body)));
        }
        case TOKEN_FINAL:
        case TOKEN_CONST:
        case TOKEN_VAR: {
            AstValInfo *val_info = NOT_NULL(parse_val_info(parser));
            AstExpr *initializer = NULL;
            if (parser_next_is(parser, TOKEN_ASSIGN)) {
                initializer = parse_expr(parser);
            }
            PARSER_EXPECT_NEXT(TOKEN_SEMICOLON, ";");
            return ast_node_decl_new_global(ast_global_decl_new_val(public_name, ast_val_decl_new(val_info, initializer)));
        }
        default:
            PARSE_ERROR(EXPECTED("external decl"));
            return NULL;
    }
}
