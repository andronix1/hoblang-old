#pragma once

#include "core/vec.h"
#include "core/slice.h"

typedef struct {
	AstType type;
	Slice name;
} AstFuncArg;

typedef struct {
	Slice name;
	AstType returning;
	AstFuncArg *args;
} AstFuncInfo;

void print_ast_func_info(FILE *stream, va_list *list);
