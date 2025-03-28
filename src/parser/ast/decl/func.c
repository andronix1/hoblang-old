#include "func.h"
#include "ast/interface/body.h"
#include "ast/interface/func_info.h"
#include "ast/node/decl.h"
#include "ast/node/decl/func.h"
#include "ast/shared/generic.h"
#include "ast/shared/func_info.h"
#include "core/not_null.h"
#include "parser/ast/shared/body.h"
#include "parser/ast/shared/func_info.h"

AstNodeDecl *parse_func_decl(Parser *parser) {
    AstGenerics *generics = NULL;
    AstFuncInfo *info = NOT_NULL(parse_func_info(parser, &generics));
        AstBody *body = NOT_NULL(parse_body(parser));
    return ast_node_decl_new_generic(info->name_loc, ast_node_decl_generic_new_func(generics, 
        ast_func_decl_new(info, body)
    ));
}
