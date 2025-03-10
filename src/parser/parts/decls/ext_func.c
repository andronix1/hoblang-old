#include "ast/private/module_node.h"
#include "parser/parts/func_info.h"

bool parse_ext_func_decl(Parser *parser, Slice name, AstExtFuncDecl *info) {
	if (!parse_func_info(parser, &info->info)) {
		return false;
	}
	info->info.public_name = name;
	return true;
}
