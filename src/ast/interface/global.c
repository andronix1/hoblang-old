#include "global.h"
#include "ast/shared/global.h"
#include "ast/shared/func_info.h"
#include "ast/shared/val_info.h"
#include "core/assert.h"

Slice ast_global_decl_info_name(AstGlobalDeclInfo *info) {
    switch (info->kind) {
        case AST_GLOBAL_DECL_INFO_FUNC: return info->func->name;
        case AST_GLOBAL_DECL_INFO_VAL: return info->val->name;
    }
    assert(0, "invalid global decl");
}

FileLocation ast_global_decl_info_decl_loc(AstGlobalDeclInfo *info) {
    switch (info->kind) {
        case AST_GLOBAL_DECL_INFO_FUNC: return info->func->name_loc;
        case AST_GLOBAL_DECL_INFO_VAL: return info->val->name_loc;
    }
    assert(0, "invalid global decl");
}
