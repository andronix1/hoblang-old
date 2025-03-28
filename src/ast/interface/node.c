#include "node.h"
#include "ast/node/decl.h"
#include "ast/shared/func_info.h"
#include "core/assert.h"

Slice ast_node_decl_generic_get_name(AstNodeDeclGeneric *decl) {
    switch (decl->kind) {
        case AST_NODE_DECL_GENERIC_FUNC: return decl->func_decl.info->name;
        case AST_NODE_DECL_GENERIC_STRUCT: return decl->struct_decl.name;
        case AST_NODE_DECL_GENERIC_TYPE_ALIAS: return decl->type_alias.name;
        case AST_NODE_DECL_GENERIC_BEHAVIOUR: return decl->behaviour.name;
    }
    assert(0, "invalid generic node decl");
}
