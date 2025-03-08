#pragma once

#include "ast/private/body.h"
#include "ast/private/func_info.h"

typedef struct AstExprAnonFun {
    AstFuncArg *args;
	AstType returning;
    AstBody body;

	SemaDecl *decl;
} AstExprAnonFun;
