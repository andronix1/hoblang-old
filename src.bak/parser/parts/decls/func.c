#include "parser/parts/generic.h"
#include "parser/private.h"
#include "ast/private/module_node.h"
#include "parser/parts/body.h"
#include "parser/parts/func_info.h"

bool parse_func_decl(Parser *parser, AstFuncDecl *decl) {
	if (!parse_func_decl_info(parser, &decl->info)) {
		return false;
	}
    decl->generics = NULL;
    if (parser_next_is(parser, TOKEN_LESS)) {
        if (!(decl->generics = parse_generic(parser))) {
            return false;
        }
    }
	if (!parse_func_type_info(parser, &decl->info.type)) {
		return false;
	}
	decl->info.public_name = decl->info.name;
	parse_body(parser, &decl->body);
	return true;
}
