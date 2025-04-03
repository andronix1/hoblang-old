#pragma once

#include "ast/interface/expr.h"
#include "ast/interface/val_info.h"
#include <llvm-c/Types.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    AstValInfo *info;
    AstExpr *initializer;
} AstValDecl;

static inline AstValDecl *ast_val_decl_new(AstValInfo *info, AstExpr *initializer) {
    AstValDecl *result = malloc(sizeof(AstValDecl));
    result->info = info;
    result->initializer = initializer;
    return result;
}

