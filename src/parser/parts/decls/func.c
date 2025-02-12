#include "../../parts.h"
#include "ast/private/module_node.h"
#include "../body.h"
#include "func_info.h"

bool parse_func_decl(Parser *parser, AstFuncDecl *decl) {
	if (!parse_func_info(parser, &decl->info)) {
		return false;
	}
	decl->info.public_name = decl->info.name;
	parse_body(parser, &decl->body);
	return true;
}
