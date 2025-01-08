#pragma once

#include <stdarg.h>
#include "core/slice.h"
#include "core/vec.h"
#include "parser/ast.h"

typedef struct {
	AstFuncArg *args;
	struct _SemaType *returning;
} SemaFunction;

typedef enum {
	PRIMITIVE_I8, PRIMITIVE_I16, PRIMITIVE_I32, PRIMITIVE_I64,
	PRIMITIVE_U8, PRIMITIVE_U16, PRIMITIVE_U32, PRIMITIVE_U64,
	PRIMITIVE_BOOL,
	PRIMITIVE_VOID
} Primitive;

typedef enum {
	SEMA_TYPE_PRIMITIVE,
	SEMA_TYPE_FUNCTION,
} SemaTypeKind;

typedef struct _SemaType {
	SemaTypeKind type;
	union {
		Primitive primitive;
		SemaFunction func;
	};
} SemaType;

extern SemaType primitives[];

bool sema_types_equals(SemaType *type, SemaType *other);
void print_sema_type(FILE* stream, va_list *list);
