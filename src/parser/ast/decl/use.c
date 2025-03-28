#include "use.h"
#include "ast/node/decl.h"
#include "ast/node/decl/use.h"
#include "ast/interface/path.h"
#include "core/not_null.h"
#include "parser/api.h"
#include "parser/ast/shared/path.h"

AstNodeDecl *parse_use(Parser *parser) {
    AstPath *path = NOT_NULL(parse_path(parser));
    AstNodeDecl *result = ast_node_decl_new_use(ast_path_loc(path), ast_use_new(path));
    PARSER_EXPECT_NEXT(TOKEN_SEMICOLON, "`;`");
    return result;
}
