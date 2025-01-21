#pragma once

#include "core/slice.h"
#include "types/struct.h"

struct _SemaType;

typedef enum {
	AST_TYPE_PATH,
	AST_TYPE_STRUCT,
	AST_TYPE_POINTER
} AstTypeKind;

typedef struct _AstType {
	AstTypeKind type;
	struct _SemaType *sema;
	union {
		AstModPath path;
		AstStructType struct_type;
		struct _AstType *ptr_to;
	};
} AstType;

void print_ast_type(FILE *stream, va_list *list);
