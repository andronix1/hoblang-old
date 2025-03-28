#include "type.h"
#include "ast/interface/type.h"
#include "ast/node/decl.h"
#include "ast/node/decl/type_alias.h"
#include "lexer/token/kind.h"
#include "parser/api.h"
#include "parser/ast/shared/generics.h"
#include "parser/ast/shared/type.h"

AstNodeDecl *parse_type_alias(Parser *parser) {
    Token *name_token = PARSER_EXPECT_NEXT(TOKEN_IDENT, "struct name");
    Slice name = name_token->ident;
    FileLocation name_loc = name_token->location;
    AstGenerics *generics = parse_generics_optional(parser);
    PARSER_EXPECT_NEXT(TOKEN_ASSIGN, "=");
    AstType *type = parse_type(parser);
    PARSER_EXPECT_NEXT(TOKEN_SEMICOLON, "`;`");
    return ast_node_decl_new_generic(name_loc, 
        ast_node_decl_generic_new_type_alias(
            generics, ast_type_alias_new(name, name_loc, type)
        )
    );
}
