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

	bool is_extension;
	Slice extension_of;
	SemaType *ext_type;
	
	Slice public_name;
	SemaScopeValueDecl *decl;
	SemaScopeValueDecl *self;
} AstFuncInfo;
