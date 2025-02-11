#pragma once

#include "sema/module/ast.h"
#include "core/vec.h"
#include "core/slice.h"
#include "type.h"

typedef struct {
	AstType type;
	Slice name;

	SemaScopeValueDecl *decl;
} AstFuncArg;

typedef struct _AstFuncInfo {
	Slice name;
	AstType returning;
	AstFuncArg *args;
	
	Slice public_name;
	SemaScopeValueDecl *decl;
} AstFuncInfo;

void print_ast_func_info(FILE *stream, va_list *list);
