#include "import.h"
#include "ast/node/decl.h"
#include "parser/api.h"

AstNodeDecl *parse_import(Parser *parser) {
    Token *file_path_token = PARSER_EXPECT_NEXT(TOKEN_STR, "file path");
    FileLocation file_path_loc = file_path_token->location;
    Slice file_path = file_path_token->str;
    PARSER_EXPECT_NEXT(TOKEN_AS, "`as`");
    Token *alias_token = PARSER_EXPECT_NEXT(TOKEN_IDENT, "alias");
    FileLocation alias_loc = alias_token->location;
    Slice alias = alias_token->ident;
    PARSER_EXPECT_NEXT(TOKEN_SEMICOLON, "`;`");
    return ast_node_decl_new_import(alias_loc, ast_import_new(
        file_path, file_path_loc,
        alias, alias_loc
    ));
}
