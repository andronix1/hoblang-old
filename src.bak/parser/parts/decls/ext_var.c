#include "lexer/token.h"
#include "parser/parts/type.h"
#include "parser/private.h"
#include "ast/private/module_node.h"

bool parse_ext_var_decl(Parser *parser, Slice name, AstExtVarDecl *info) {
    info->ext_name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "var name")->ident;
    info->public_name = name;
    PARSER_EXPECT_NEXT(TOKEN_COLON, "external var type");
	return (info->type = parse_type(parser));
}
