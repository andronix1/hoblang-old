#pragma once

#include "ast/interface/global.h"
#include "ast/node/decl/func.h"
#include "ast/node/decl/val.h"

typedef enum {
    AST_GLOBAL_DECL_FUNC,
    AST_GLOBAL_DECL_VAL,
} AstGlobalDeclKind;

typedef struct AstGlobalDecl {
    AstGlobalDeclKind kind;
    Slice public_name;
    union {
        AstFuncDecl *func_decl;
        AstValDecl *val_decl;
    };
} AstGlobalDecl;

static inline AstGlobalDecl ast_global_decl_new_func(Slice public_name, AstFuncDecl *decl) {
    AstGlobalDecl result = {
        .kind = AST_GLOBAL_DECL_FUNC,
        .public_name = public_name,
        .func_decl = decl
    };
    return result;
}

static inline AstGlobalDecl ast_global_decl_new_val(Slice public_name, AstValDecl *decl) {
    AstGlobalDecl result = {
        .kind = AST_GLOBAL_DECL_VAL,
        .public_name = public_name,
        .val_decl = decl
    };
    return result;
}
