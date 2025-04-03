#pragma once

#include "ast/interface/func_info.h"
#include "ast/interface/val_info.h"
#include "core/slice/slice.h"
#include <stdlib.h>

typedef enum {
    AST_GLOBAL_DECL_INFO_FUNC,
    AST_GLOBAL_DECL_INFO_VAL,
} AstGlobalDeclInfoKind;

typedef struct AstGlobalDeclInfo {
    AstGlobalDeclInfoKind kind;
    Slice public_name;
    union {
        AstFuncInfo *func;
        AstValInfo *val;
    };
} AstGlobalDeclInfo;

static inline AstGlobalDeclInfo *ast_global_decl_info_new_val(Slice public_name, AstValInfo *val) {
    AstGlobalDeclInfo *result = malloc(sizeof(AstGlobalDeclInfo));
    result->kind = AST_GLOBAL_DECL_INFO_FUNC;
    result->public_name = public_name;
    result->val = val;
    return result;
}

static inline AstGlobalDeclInfo *ast_global_decl_info_new_func(Slice public_name, AstFuncInfo *func) {
    AstGlobalDeclInfo *result = malloc(sizeof(AstGlobalDeclInfo));
    result->kind = AST_GLOBAL_DECL_INFO_FUNC;
    result->public_name = public_name;
    result->func = func;
    return result;
}

