#pragma once

#include "core/slice.h"

struct _SemaType;

typedef enum {
	AST_TYPE_IDENT
} AstTypeKind;

typedef struct {
	AstTypeKind type;
	struct _SemaType *sema;
	union {
		Slice ident;
	};
} AstType;

void print_ast_type(FILE *stream, va_list *list);
