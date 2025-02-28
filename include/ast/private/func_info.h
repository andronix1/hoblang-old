#pragma once

#include "sema/module/decls.h"
#include "core/vec.h"
#include "core/slice.h"
#include "type.h"

typedef struct {
	AstType type;
	Slice name;

	SemaDecl *decl;
} AstFuncArg;

typedef struct {
	AstType of;
} AstExtFuncInfo;

typedef struct AstFuncInfo {
	Slice name;
	AstType returning;
	AstFuncArg *args;

	bool is_extension;
    AstExtFuncInfo ext;
	
	Slice public_name;
	SemaDecl *decl;
	SemaDecl *self;
} AstFuncInfo;
