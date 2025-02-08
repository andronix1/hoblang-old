#include "../../parsers.h"

bool parse_ext_func_decl(Parser *parser, AstExtFuncDecl *info) {
	PARSER_EXPECT_NEXT(TOKEN_OPENING_CIRCLE_BRACE, "external name opening");
	info->ext_name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "external name")->ident;
	PARSER_EXPECT_NEXT(TOKEN_CLOSING_CIRCLE_BRACE, "external name closing");
	PARSER_EXPECT_NEXT(TOKEN_FUN, "function");
	if (!parse_func_info(parser, &info->info)) {
		return false;
	}
	info->info.public_name = info->ext_name;
	return true;
}
