#pragma once

#include "core/slice.h"

typedef enum {
	AST_TYPE_IDENT
} AstTypeKind;

typedef struct {
	AstTypeKind type;
	union {
		Slice ident;
	};
} AstType;

void print_ast_type(FILE *stream, va_list *list);
