#pragma once

#include "ast/interface/func_info.h"
#include "ast/interface/body.h"

typedef struct {
    AstFuncInfo *info;
    AstBody *body;
} AstFuncDecl;

static inline AstFuncDecl ast_func_decl_new(AstFuncInfo *info, AstBody *body) {
    AstFuncDecl result = {
        .info = info,
        .body = body,
    };
    return result;
}
