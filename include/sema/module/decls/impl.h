#pragma once

#include <stdbool.h>
#include <llvm-c/Core.h>
#include "core/slice.h"
#include "sema/type.h"
#include "ast/api/expr.h"
#include "sema/module.h"

typedef enum {
    SEMA_SCOPE_DECL_TYPE,
    SEMA_SCOPE_DECL_MODULE,
    SEMA_SCOPE_DECL_VALUE,
} SemaScopeDeclType;

typedef struct SemaScopeValueDecl {
    SemaType *type;
    // TODO: abstract const value
    AstExpr *integer_expr;
    
    // sema
    bool constant;
    LLVMValueRef llvm_value; // TODO: i don't like it)
} SemaScopeValueDecl;

typedef struct SemaScopeDecl {
    SemaScopeDeclType type;
    Slice name;
    SemaType *in_type;
    union {
        SemaType *sema_type;
        SemaScopeValueDecl value_decl;
        SemaModule *module;
    };
} SemaScopeDecl;

