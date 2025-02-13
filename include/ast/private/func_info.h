#pragma once

#include "sema/module/decls.h"
#include "core/vec.h"
#include "core/slice.h"
#include "type.h"

typedef struct {
	AstType type;
	Slice name;

	SemaScopeValueDecl *decl;
} AstFuncArg;

typedef struct AstFuncInfo {
	Slice name;
	AstType returning;
	AstFuncArg *args;
	
	Slice public_name;
	SemaScopeValueDecl *decl;
} AstFuncInfo;
