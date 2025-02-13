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

#define SEMA_TYPE_PRIMITIVE_IMPL(name, kind) \
    SemaType *sema_type_primitive_##name() { \
        static SemaType result = { .type = SEMA_TYPE_PRIMITIVE, .primitive = kind }; \
        return &result; \
    }

SEMA_TYPE_PRIMITIVE_IMPL(u8, PRIMITIVE_U8)
SEMA_TYPE_PRIMITIVE_IMPL(u16, PRIMITIVE_U16)
SEMA_TYPE_PRIMITIVE_IMPL(u32, PRIMITIVE_U32)
SEMA_TYPE_PRIMITIVE_IMPL(u64, PRIMITIVE_U64)
SEMA_TYPE_PRIMITIVE_IMPL(i8, PRIMITIVE_I8)
SEMA_TYPE_PRIMITIVE_IMPL(i16, PRIMITIVE_I16)
SEMA_TYPE_PRIMITIVE_IMPL(i32, PRIMITIVE_I32)
SEMA_TYPE_PRIMITIVE_IMPL(i64, PRIMITIVE_I64)
SEMA_TYPE_PRIMITIVE_IMPL(bool, PRIMITIVE_BOOL)
SEMA_TYPE_PRIMITIVE_IMPL(void, PRIMITIVE_VOID)
