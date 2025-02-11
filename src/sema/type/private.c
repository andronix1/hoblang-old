#include "private.h"

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
