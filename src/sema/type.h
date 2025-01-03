#pragma once

#include "core/fatptr.h"
#include "core/vec.h"

struct _Type;

typedef struct {
	FatPtr name;
	struct _Type *type;
} TypeFuncArg;

typedef Vec TypeFuncArgs;

typedef struct {
	TypeFuncArgs args;
	struct _Type *returning; // may be NULL
} TypeFunction;

typedef enum {
	PRIMITIVE_I8, PRIMITIVE_I16, PRIMITIVE_I32, PRIMITIVE_I64,
	PRIMITIVE_U8, PRIMITIVE_U16, PRIMITIVE_U32, PRIMITIVE_U64,
	PRIMITIVE_BOOL,
	PRIMITIVE_VOID
} Primitive;

typedef enum {
	TYPE_PRIMITIVE,
	TYPE_FUNCTION,
} TypeKind;

typedef struct _Type {
	TypeKind type;
	union {
		Primitive primitive;
		TypeFunction func;
	};
} Type;

#define TYPE_PRIMITIVE(kind) [kind] = { .type = TYPE_PRIMITIVE, .primitive = kind }

extern Type primitives[];

bool types_equals(Type *type, Type *other);
