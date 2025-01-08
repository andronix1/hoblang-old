#include "../../parsers.h"

bool parse_ext_func_decl(Parser *parser, AstExtFuncDecl *info) {
	parse_exp_next(TOKEN_FUN, "function");
	if (!parse_func_info(parser, &info->info)) {
		return false;
	}
	return true;
}
