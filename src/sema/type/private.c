#include "sema/type/private.h"
#include <malloc.h>

SemaType *sema_type_new_array(size_t length, SemaType *of) {
	SemaType *result = malloc(sizeof(SemaType));
	result->type = SEMA_TYPE_ARRAY;
	result->array.of = of;
	result->array.length = length;
	return result;
}

SemaType *sema_type_new_slice(SemaType *of) {
	SemaType *result = malloc(sizeof(SemaType));
	result->type = SEMA_TYPE_SLICE;
	result->slice_of = of;
	return result;
}

SemaType *sema_type_new_pointer(SemaType *to) {
	SemaType *result = malloc(sizeof(SemaType));
	result->type = SEMA_TYPE_POINTER;
	result->ptr_to = to;
	return result;
}

SemaType *sema_type_new_func(SemaType *returning, SemaType **args) {
	SemaType *result = malloc(sizeof(SemaType));
	result->type = SEMA_TYPE_FUNCTION;
	result->func.returning = returning;
	result->func.args = args;
	return result;
}

SemaType *sema_type_new_struct(AstStructDef *struct_def) {
	SemaType *result = malloc(sizeof(SemaType));
	result->type = SEMA_TYPE_STRUCT;
	result->struct_def = struct_def;
	return result;
}

#define SEMA_TYPE_SIMPLE_PRIMITIVE_IMPL(name, kind) \
    SemaType *sema_type_primitive_##name() { \
        static SemaType result = { .type = SEMA_TYPE_PRIMITIVE, .primitive = { .type = kind } }; \
        return &result; \
    }

#define SEMA_TYPE_INT_PRIMITIVE_IMPL(name, kind) \
    SemaType *sema_type_primitive_##name() { \
        static SemaType result = { .type = SEMA_TYPE_PRIMITIVE, .primitive = { .type = SEMA_PRIMITIVE_INT, .integer = kind } }; \
        return &result; \
    }

#define SEMA_TYPE_FLOAT_PRIMITIVE_IMPL(name, kind) \
    SemaType *sema_type_primitive_##name() { \
        static SemaType result = { .type = SEMA_TYPE_PRIMITIVE, .primitive = { .type = SEMA_PRIMITIVE_FLOAT, .float_type = kind } }; \
        return &result; \
    }

SEMA_TYPE_INT_PRIMITIVE_IMPL(u8, SEMA_PRIMITIVE_UINT8)
SEMA_TYPE_INT_PRIMITIVE_IMPL(u16, SEMA_PRIMITIVE_UINT16)
SEMA_TYPE_INT_PRIMITIVE_IMPL(u32, SEMA_PRIMITIVE_UINT32)
SEMA_TYPE_INT_PRIMITIVE_IMPL(u64, SEMA_PRIMITIVE_UINT64)
SEMA_TYPE_INT_PRIMITIVE_IMPL(i8, SEMA_PRIMITIVE_INT8)
SEMA_TYPE_INT_PRIMITIVE_IMPL(i16, SEMA_PRIMITIVE_INT16)
SEMA_TYPE_INT_PRIMITIVE_IMPL(i32, SEMA_PRIMITIVE_INT32)
SEMA_TYPE_INT_PRIMITIVE_IMPL(i64, SEMA_PRIMITIVE_INT64)
SEMA_TYPE_FLOAT_PRIMITIVE_IMPL(f32, SEMA_PRIMITIVE_FLOAT32)
SEMA_TYPE_FLOAT_PRIMITIVE_IMPL(f64, SEMA_PRIMITIVE_FLOAT64)
SEMA_TYPE_SIMPLE_PRIMITIVE_IMPL(bool, SEMA_PRIMITIVE_BOOL)
SEMA_TYPE_SIMPLE_PRIMITIVE_IMPL(void, SEMA_PRIMITIVE_VOID)
