#include "external.h"
#include "ast/shared/val_info.h"
#include "ast/shared/func_info.h"
#include "ast/node/decl.h"
#include "ast/node/decl/external.h"
#include "core/not_null.h"
#include "lexer/token/kind.h"
#include "parser/api.h"
#include "parser/ast/shared/func_info.h"
#include "parser/ast/shared/val_info.h"

AstNodeDecl *parse_external(Parser *parser) {
    PARSER_EXPECT_NEXT(TOKEN_OPENING_CIRCLE_BRACE, "`(`");
    Slice public_name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "public name")->ident;
    PARSER_EXPECT_NEXT(TOKEN_CLOSING_CIRCLE_BRACE, "`)`");
    switch (parser_next(parser)->kind) {
        case TOKEN_FUN: {
            AstFuncInfo *func_info = NOT_NULL(parse_func_info(parser, NULL));
            return ast_node_decl_new_external(func_info->name_loc, ast_external_decl_new_func(public_name, func_info));
        }
        case TOKEN_FINAL:
        case TOKEN_CONST:
        case TOKEN_VAR: {
            AstValInfo *val_info = NOT_NULL(parse_val_info(parser));
            parser_skip_next(parser);
            return ast_node_decl_new_external(val_info->name_loc, ast_external_decl_new_val(public_name, val_info));
        }
        default:
            PARSE_ERROR(EXPECTED("external decl"));
            return NULL;
    }
}
