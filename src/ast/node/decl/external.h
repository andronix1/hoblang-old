#pragma once

#include "ast/interface/func_info.h"
#include "ast/interface/val_info.h"
#include "core/slice/slice.h"

typedef enum {
    AST_EXTERNAL_DECL_FUNC,
    AST_EXTERNAL_DECL_VAL,
} AstExternalDeclKind;

typedef struct {
    AstExternalDeclKind kind;
    Slice public_name;
    union {
        AstFuncInfo *func;
        AstValInfo *val;
    };
} AstExternalDecl;

static inline AstExternalDecl ast_external_decl_new_val(Slice public_name, AstValInfo *info) {
    AstExternalDecl result = {
        .kind = AST_EXTERNAL_DECL_VAL,
        .val = info,
        .public_name = public_name
    };
    return result;
}

static inline AstExternalDecl ast_external_decl_new_func(Slice public_name, AstFuncInfo *info) {
    AstExternalDecl result = {
        .kind = AST_EXTERNAL_DECL_FUNC,
        .func = info,
        .public_name = public_name
    };
    return result;
}
