#include "../../parsers.h"

bool parse_func_decl(Parser *parser, AstFuncDecl *decl) {
	if (!parse_func_info(parser, &decl->info)) {
		return false;
	}
	parse_body(parser, &decl->body);
	return true;
}
