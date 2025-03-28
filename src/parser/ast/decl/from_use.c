#include "from_use.h"
#include "ast/interface/node.h"
#include "ast/interface/path.h"
#include "ast/node/decl.h"
#include "ast/node/decl/from_use.h"
#include "ast/shared/path.h"
#include "core/not_null.h"
#include "core/vec.h"
#include "lexer/token/kind.h"
#include "parser/api.h"
#include "parser/ast/shared/path.h"

AstNodeDecl *parse_from_use(Parser *parser, FileLocation from_loc) {
    AstPath *from = parse_path(parser);
    PARSER_EXPECT_NEXT(TOKEN_USE, "`use`");
    AstNodeDecl *result = NULL;
    if (parser_next_is(parser, TOKEN_MULTIPLY)) {
        result = ast_node_decl_new_from_use(from_loc, ast_from_use_new_all(from, parser_token(parser)->location));
        PARSER_EXPECT_NEXT(TOKEN_SEMICOLON, "`;`");
    } else {
        AstPath **list = vec_new(AstPath*);
        while (parser_next_is_not(parser, TOKEN_SEMICOLON)) {
            list = vec_push_dir(list, NOT_NULL(parse_path(parser)));
            switch (parser_next(parser)->kind) {
                case TOKEN_COMMA: break;
                case TOKEN_SEMICOLON: break;
                default:
                    PARSE_ERROR(EXPECTED("comma or semicolon"));
                    return NULL;
            }
        }
        result = ast_node_decl_new_from_use(from_loc, ast_from_use_new_list(from, list));
    }
    return result;
}
