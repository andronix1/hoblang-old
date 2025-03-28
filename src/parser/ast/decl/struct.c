#include "struct.h"
#include "ast/node/decl.h"
#include "core/not_null.h"
#include "core/slice/slice.h"
#include "core/vec.h"
#include "parser/api.h"
#include "ast/node/decl/struct.h"
#include "parser/ast/shared/generics.h"
#include "parser/ast/shared/type.h"

AstNodeDecl *parse_struct_decl(Parser *parser) {
    Token *name_token = PARSER_EXPECT_NEXT(TOKEN_IDENT, "struct name");
    Slice name = name_token->ident;
    FileLocation name_loc = name_token->location;
    AstGenerics *generics = parse_generics_optional(parser);
    PARSER_EXPECT_NEXT(TOKEN_OPENING_FIGURE_BRACE, "{");
    AstStructDeclField *fields = vec_new(AstStructDeclField);
    while (parser_next_is_not(parser, TOKEN_CLOSING_FIGURE_BRACE)) {
        bool public = parser_next_is(parser, TOKEN_PUBLIC);
        Token *name_token = PARSER_EXPECT_NEXT(TOKEN_IDENT, "struct name");
        Slice name = name_token->ident;
        FileLocation name_loc = name_token->location;
        PARSER_EXPECT_NEXT(TOKEN_COLON , "colon");
        AstType *type = NOT_NULL(parse_type(parser));
        fields = vec_push_dir(fields, ast_struct_decl_field_new(public, name, name_loc, type));
        switch (parser_next(parser)->kind) {
            case TOKEN_COMMA: break;
            case TOKEN_CLOSING_FIGURE_BRACE:
                parser_skip_next(parser);
                break;
            default:
                PARSE_ERROR(EXPECTED("comma or `}`"));
                break;
        }
    }
    return ast_node_decl_new_generic(name_loc, ast_node_decl_generic_new_struct(
        generics,
        ast_struct_decl_new(name, name_loc, fields)
    ));
}
