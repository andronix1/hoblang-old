#pragma once

#include <stddef.h>
#include "ast/api/type.h"
#include "sema/type/api.h"
#include "sema/type.h"

typedef struct SemaType SemaType;

typedef struct SemaFunction {
	SemaType **args;
	SemaType *returning;
} SemaFunction;

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

#define SEMA_TYPE_PRIMITIVE(name, kind) \
    SemaType *sema_type_primitive_##name();

SEMA_TYPE_PRIMITIVE(u8, PRIMITIVE_U8)
SEMA_TYPE_PRIMITIVE(u16, PRIMITIVE_U16)
SEMA_TYPE_PRIMITIVE(u32, PRIMITIVE_U32)
SEMA_TYPE_PRIMITIVE(u64, PRIMITIVE_U64)
SEMA_TYPE_PRIMITIVE(i8, PRIMITIVE_I8)
SEMA_TYPE_PRIMITIVE(i16, PRIMITIVE_I16)
SEMA_TYPE_PRIMITIVE(i32, PRIMITIVE_I32)
SEMA_TYPE_PRIMITIVE(i64, PRIMITIVE_I64)
SEMA_TYPE_PRIMITIVE(bool, PRIMITIVE_BOOL)
SEMA_TYPE_PRIMITIVE(void, PRIMITIVE_VOID)
