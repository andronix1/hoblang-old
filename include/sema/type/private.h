#pragma once

#include <stddef.h>
#include "ast/api/type.h"
#include "ast/api/module_node.h"
#include "sema/module/behaviour/impl.h"
#include "sema/type/api.h"
#include "func.h"

typedef enum {
	SEMA_TYPE_PRIMITIVE,
	SEMA_TYPE_OPTIONAL,
	SEMA_TYPE_POINTER,
	SEMA_TYPE_FUNCTION,
	SEMA_TYPE_STRUCT,
	SEMA_TYPE_SLICE,
	SEMA_TYPE_ARRAY,
	SEMA_TYPE_GENERIC,
} SemaTypeKind;

typedef struct {
	SemaType *of;
	size_t length;
} SemaArrayType;

typedef enum {
	SEMA_PRIMITIVE_INT,
	SEMA_PRIMITIVE_BOOL,
	SEMA_PRIMITIVE_FLOAT,
	SEMA_PRIMITIVE_VOID
} SemaPrimitiveType;

typedef enum {
	SEMA_PRIMITIVE_INT8,
	SEMA_PRIMITIVE_INT16,
	SEMA_PRIMITIVE_INT32,
	SEMA_PRIMITIVE_INT64,
	SEMA_PRIMITIVE_UINT8,
	SEMA_PRIMITIVE_UINT16,
	SEMA_PRIMITIVE_UINT32,
	SEMA_PRIMITIVE_UINT64,
} SemaPrimitiveIntType;

typedef enum {
	SEMA_PRIMITIVE_FLOAT32,
	SEMA_PRIMITIVE_FLOAT64,
} SemaPrimitiveFloatType;

typedef struct {
	SemaPrimitiveType type;
	union {
		SemaPrimitiveIntType integer;
		SemaPrimitiveFloatType float_type;
	};
} SemaPrimitive;

typedef struct SemaTypeGeneric {
	SemaBehaviour *behaviour;

    SemaType *replace;
} SemaTypeGeneric;

typedef struct SemaType {
	SemaTypeKind type;
	union {
		SemaPrimitive primitive;
		SemaFunction func;
		SemaType *ptr_to;
		SemaType *slice_of;
		SemaType *optional_of;
		AstStructDef *struct_def;
		SemaArrayType array;
        SemaTypeGeneric generic;
	};
} SemaType;

SemaType *sema_type_new_array(size_t length, SemaType *of);
SemaType *sema_type_new_slice(SemaType *of);
SemaType *sema_type_new_pointer(SemaType *to); 
SemaType *sema_type_new_func(SemaType *returning, SemaType **args);
SemaType *sema_type_new_struct(AstStructDef *struct_def);
SemaType *sema_type_new_optional(SemaType *of);
SemaType *sema_type_new_generic(SemaBehaviour *behaviour);

#define SEMA_TYPE_PRIMITIVE(name) SemaType *sema_type_primitive_##name();

SEMA_TYPE_PRIMITIVE(u8)
SEMA_TYPE_PRIMITIVE(bool)
SEMA_TYPE_PRIMITIVE(void)
