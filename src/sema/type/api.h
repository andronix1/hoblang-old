#pragma once

#include <stdbool.h>
#include "type.h"

typedef struct SemaFunction SemaFunction;

typedef enum {
	PRIMITIVE_I8, PRIMITIVE_I16, PRIMITIVE_I32, PRIMITIVE_I64,
	PRIMITIVE_U8, PRIMITIVE_U16, PRIMITIVE_U32, PRIMITIVE_U64,
	PRIMITIVE_BOOL,
	PRIMITIVE_VOID
} Primitive;

bool sema_types_equals(SemaType *type, SemaType *other);
bool sema_type_is_primitive(SemaType *type, Primitive primitive);
