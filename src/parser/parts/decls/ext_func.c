#include "ast/private/module_node.h"
#include "parser/parts/func_info.h"

bool parse_ext_func_decl(Parser *parser, Slice name, AstExtFuncDecl *info) {
	if (!parse_func_decl_info(parser, &info->info)) {
		return false;
	}
	if (!parse_func_type_info(parser, &info->info.type)) {
		return false;
	}
	info->info.public_name = name;
	return true;
}
