#pragma once

#include "ast/private/body.h"
#include "ast/private/func_info.h"

typedef struct AstExprAnonFun {
    AstFuncTypeInfo info;
    AstBody body;

	SemaDecl *decl;
} AstExprAnonFun;
