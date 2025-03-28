#pragma once

#include "ast/node/decl/struct.h"
#include "sema/interface/behaviour.h"
#include "sema/interface/type.h"
#include <stdlib.h>

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
} SemaArrayKind;

typedef enum {
	SEMA_PRIMITIVE_INT,
	SEMA_PRIMITIVE_BOOL,
	SEMA_PRIMITIVE_FLOAT,
	SEMA_PRIMITIVE_VOID
} SemaPrimitiveKind;

typedef enum {
	SEMA_PRIMITIVE_INT8,
	SEMA_PRIMITIVE_INT16,
	SEMA_PRIMITIVE_INT32,
	SEMA_PRIMITIVE_INT64,
	SEMA_PRIMITIVE_UINT8,
	SEMA_PRIMITIVE_UINT16,
	SEMA_PRIMITIVE_UINT32,
	SEMA_PRIMITIVE_UINT64,
} SemaPrimitiveIntKind;

typedef enum {
	SEMA_PRIMITIVE_FLOAT32,
	SEMA_PRIMITIVE_FLOAT64,
} SemaPrimitiveFloatKind;

typedef struct {
	SemaPrimitiveKind kind;
	union {
		SemaPrimitiveIntKind integer;
		SemaPrimitiveFloatKind float_type;
	};
} SemaPrimitive;

typedef struct SemaTypeGeneric {
    SemaBehaviour *behaviour;
} SemaTypeGeneric;

typedef struct SemaFunction {
	SemaType **args;
	SemaType *returns;
} SemaFunction;

typedef struct SemaType {
	SemaTypeKind kind;
	union {
		SemaPrimitive primitive;
		SemaFunction func;
		SemaType *pointer_to;
		SemaType *slice_of;
		SemaType *optional_of;
		AstStructDecl *struct_decl;
		SemaArrayKind array;
		SemaTypeGeneric generic;
	};
} SemaType;

static inline SemaType *sema_type_new_array(size_t length, SemaType *of) {
    SemaType *result = malloc(sizeof(SemaType));
    result->kind = SEMA_TYPE_ARRAY;
    result->array.length = length;
    result->array.of = of;
    return result;
}

static inline SemaType *sema_type_new_generic(SemaBehaviour *behaviour) {
    SemaType *result = malloc(sizeof(SemaType));
    result->kind = SEMA_TYPE_GENERIC;
    result->generic.behaviour = behaviour;
    return result;
}

static inline SemaType *sema_type_new_slice(SemaType *of) {
    SemaType *result = malloc(sizeof(SemaType));
    result->kind = SEMA_TYPE_SLICE;
    result->slice_of = of;
    return result;
}

static inline SemaType *sema_type_new_pointer(SemaType *to) {
    SemaType *result = malloc(sizeof(SemaType));
    result->kind = SEMA_TYPE_POINTER;
    result->pointer_to = to;
    return result;
}

static inline SemaType *sema_type_new_func(SemaType **args, SemaType *returns) {
    SemaType *result = malloc(sizeof(SemaType));
    result->kind = SEMA_TYPE_FUNCTION;
    result->func.args = args;
    result->func.returns = returns;
    return result;
}

static inline SemaType *sema_type_new_struct(AstStructDecl *struct_decl) {
    SemaType *result = malloc(sizeof(SemaType));
    result->kind = SEMA_TYPE_STRUCT;
    result->struct_decl = struct_decl;
    return result;
}

static inline SemaType *sema_type_new_optional(SemaType *of) {
    SemaType *result = malloc(sizeof(SemaType));
    result->kind = SEMA_TYPE_STRUCT;
    result->optional_of = of;
    return result;
}

#define _SEMA_TYPE_PRIMITIVE_CONSTR(name, prim, KIND_NAME, PRIM_NAME) \
    static inline SemaType *sema_type_primitive_##name() { \
        static SemaType result = { \
            .kind = SEMA_TYPE_PRIMITIVE, \
            .primitive = { \
                .kind = SEMA_PRIMITIVE_##KIND_NAME, \
                .prim = SEMA_PRIMITIVE_##PRIM_NAME \
            } \
        }; \
        return &result; \
    }

_SEMA_TYPE_PRIMITIVE_CONSTR(i8, integer, INT, INT8);
_SEMA_TYPE_PRIMITIVE_CONSTR(i16, integer, INT, INT16);
_SEMA_TYPE_PRIMITIVE_CONSTR(i32, integer, INT, INT32);
_SEMA_TYPE_PRIMITIVE_CONSTR(i64, integer, INT, INT64);
_SEMA_TYPE_PRIMITIVE_CONSTR(u8, integer, INT, UINT8);
_SEMA_TYPE_PRIMITIVE_CONSTR(u16, integer, INT, UINT16);
_SEMA_TYPE_PRIMITIVE_CONSTR(u32, integer, INT, UINT32);
_SEMA_TYPE_PRIMITIVE_CONSTR(u64, integer, INT, UINT64);
_SEMA_TYPE_PRIMITIVE_CONSTR(f32, float_type, FLOAT, FLOAT32);
_SEMA_TYPE_PRIMITIVE_CONSTR(f64, float_type, FLOAT, FLOAT64);

static inline SemaType *sema_type_primitive_bool() {
    static SemaType result = {
        .kind = SEMA_TYPE_PRIMITIVE,
        .primitive = { .kind = SEMA_PRIMITIVE_BOOL }
    };
    return &result;
}

static inline SemaType *sema_type_primitive_void() {
    static SemaType result = {
        .kind = SEMA_TYPE_PRIMITIVE,
        .primitive = { .kind = SEMA_PRIMITIVE_VOID }
    };
    return &result;
}

