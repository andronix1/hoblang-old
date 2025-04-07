#pragma once

#include "ast/interface/type.h"
#include "ast/interface/body.h"
#include "core/location.h"
#include "core/slice/slice.h"
#include "sema/interface/decl.h"

typedef struct {
    Slice name;
    FileLocation loc;
    AstType *type; // NULLABLE
    
    struct {
        SemaDecl *decl;
    } sema;
} AstExprAnonFuncArg;

typedef struct {
    AstExprAnonFuncArg *args;
    AstType *returns; // NULLABLE
    AstBody *body;
} AstExprAnonFunc;

static inline AstExprAnonFuncArg ast_expr_anon_fun_arg_new(FileLocation loc, Slice name, AstType *type) {
    AstExprAnonFuncArg result = {
        .name = name,
        .loc = loc,
        .type = type
    };
    return result;
}
