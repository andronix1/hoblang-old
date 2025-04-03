#pragma once

#include "ast/interface/func_info.h"
#include "ast/interface/body.h"
#include "sema/decl.h"
#include <llvm-c/Types.h>
#include <stdlib.h>

typedef struct {
    AstFuncInfo *info;
    AstBody *body;

    struct {
        SemaDecl *self;
    } sema;
} AstFuncDecl;

static inline AstFuncDecl *ast_func_decl_new(AstFuncInfo *info, AstBody *body) {
    AstFuncDecl *result = malloc(sizeof(AstFuncDecl));
    result->info = info;
    result->body = body;
    return result;
}
