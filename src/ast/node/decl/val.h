#pragma once

#include "ast/interface/expr.h"
#include "ast/interface/val_info.h"

typedef struct {
    AstValInfo *info;
    AstExpr *initializer;
} AstValDecl;

static inline AstValDecl ast_val_decl_new(AstValInfo *info, AstExpr *initializer) {
    AstValDecl result = {
        .info = info,
        .initializer = initializer,
    };
    return result;
}

