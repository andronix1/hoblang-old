#pragma once

#include "core/slice.h"

struct _SemaType;

typedef enum {
	AST_TYPE_PATH,
	AST_TYPE_POINTER
} AstTypeKind;

typedef struct _AstType {
	AstTypeKind type;
	struct _SemaType *sema;
	union {
		AstModPath path;
		struct _AstType *ptr_to;
	};
} AstType;

void print_ast_type(FILE *stream, va_list *list);
