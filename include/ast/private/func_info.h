#pragma once

#include "core/location.h"
#include "sema/module/decls.h"
#include "core/vec.h"
#include "core/slice.h"
#include "ast/api/type.h"

typedef struct AstFuncArg {
	AstType *type;
	Slice name;
    FileLocation loc;

	SemaDecl *decl;
} AstFuncArg;

typedef struct {
	AstType *of;
} AstExtFuncInfo;

typedef struct AstFuncTypeInfo {
	AstType *returning;
	AstFuncArg *args;
} AstFuncTypeInfo;

typedef struct AstFuncInfo {
	Slice name;
    AstFuncTypeInfo type;

	bool is_extension;
    AstExtFuncInfo ext;
	
	Slice public_name;
	SemaDecl *decl;
	SemaDecl *self;
} AstFuncInfo;
