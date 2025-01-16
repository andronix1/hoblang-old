#include "../../parsers.h"

bool parse_ext_func_decl(Parser *parser, AstExtFuncDecl *info) {
	parse_exp_next(TOKEN_OPENING_CIRCLE_BRACE, "external name opening");
	parse_exp_next(TOKEN_IDENT, "external name");
	info->ext_name = parser->token->ident;
	parse_exp_next(TOKEN_CLOSING_CIRCLE_BRACE, "external name closing");
	parse_exp_next(TOKEN_FUN, "function");
	if (!parse_func_info(parser, &info->info)) {
		return false;
	}
	info->info.public_name = info->ext_name;
	return true;
}
