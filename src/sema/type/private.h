#pragma once

#include <stddef.h>
#include "ast/type.h"
#include "api.h"

typedef struct SemaType SemaType;

typedef struct SemaFunction {
	SemaType **args;
	SemaType *returning;
} SemaFunction;

typedef enum {
	PRIMITIVE_I8, PRIMITIVE_I16, PRIMITIVE_I32, PRIMITIVE_I64,
	PRIMITIVE_U8, PRIMITIVE_U16, PRIMITIVE_U32, PRIMITIVE_U64,
	PRIMITIVE_BOOL,
	PRIMITIVE_VOID
} Primitive;

typedef enum {
	SEMA_TYPE_PRIMITIVE,
	SEMA_TYPE_POINTER,
	SEMA_TYPE_FUNCTION,
	SEMA_TYPE_STRUCT,
	SEMA_TYPE_SLICE,
	SEMA_TYPE_ARRAY,
} SemaTypeKind;

typedef struct {
	SemaType *of;
	size_t length;
} SemaArrayType;

typedef struct SemaType {
	SemaTypeKind type;
	union {
		Primitive primitive;
		SemaFunction func;
		SemaType *ptr_to;
		SemaType *slice_of;
		AstStructType *struct_type;
		SemaArrayType array;
	};
} SemaType;

SemaType *sema_type_new_array(size_t length, SemaType *of);
SemaType *sema_type_new_slice(SemaType *of);
SemaType *sema_type_new_pointer(SemaType *to); 
SemaType *sema_type_new_func(SemaType *returning, SemaType **args);

#define TYPE_PRIMITIVE(kind) [kind] = { .type = SEMA_TYPE_PRIMITIVE, .primitive = kind }

static SemaType primitives[] = {
	TYPE_PRIMITIVE(PRIMITIVE_U8),
	TYPE_PRIMITIVE(PRIMITIVE_U16),
	TYPE_PRIMITIVE(PRIMITIVE_U32),
	TYPE_PRIMITIVE(PRIMITIVE_U64),
	TYPE_PRIMITIVE(PRIMITIVE_I8),
	TYPE_PRIMITIVE(PRIMITIVE_I16),
	TYPE_PRIMITIVE(PRIMITIVE_I32),
	TYPE_PRIMITIVE(PRIMITIVE_I64),
	TYPE_PRIMITIVE(PRIMITIVE_BOOL),
	TYPE_PRIMITIVE(PRIMITIVE_VOID),
};
