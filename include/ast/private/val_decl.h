#pragma once

#include "ast/api/expr.h"
#include "ast/api/type.h"
#include "core/location.h"
#include "core/slice.h"
#include "sema/module/decls/decls.h"
#include "sema/type.h"

typedef enum {
    AST_VAL_DECL_CONST,
    AST_VAL_DECL_VAR,
    AST_VAL_DECL_FINAL,
} AstValDeclType;

typedef struct AstValDecl {
    AstValDeclType type;
    FileLocation loc;
    Slice name;
    AstType *spec_type;
    AstExpr *initializer;

    struct {
        SemaDecl *decl;
    } sema;
} AstValDecl;
