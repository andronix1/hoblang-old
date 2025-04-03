#include "global_decl.h"
#include "ast/shared/global.h"
#include "ast/node/decl/global.h"
#include "ast/shared/func_info.h"
#include "ast/shared/val_info.h"
#include "ast/node/decl/func.h"
#include "ast/node/decl/val.h"
#include "core/assert.h"

Slice ast_global_decl_name(AstGlobalDecl *info) {
    switch (info->kind) {
        case AST_GLOBAL_DECL_FUNC: return info->func_decl->info->name;
        case AST_GLOBAL_DECL_VAL: return info->val_decl->info->name;
    }
    assert(0, "invalid global decl");
}

FileLocation ast_global_decl_decl_loc(AstGlobalDecl *info) {
    switch (info->kind) {
        case AST_GLOBAL_DECL_INFO_FUNC: return info->func_decl->info->name_loc;
        case AST_GLOBAL_DECL_INFO_VAL: return info->val_decl->info->name_loc;
    }
    assert(0, "invalid global decl");
}
