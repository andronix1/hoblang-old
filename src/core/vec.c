#include "vec.h"

Vec vec_new_sized(size_t esize) {
	Vec vec = {
		.buffer = fatptr_empty(),
		.len = 0, 
		.esize = esize
	};
	return vec;
}

#define VEC_MUL_FROM 10 
size_t vec_multiplier(size_t len) {
	if (len >= VEC_MUL_FROM) {
		return 1;
	}
	return VEC_MUL_FROM - len;
}

void vec_push(Vec *vec, void *ptr) {
	vec->len++;
	if (vec->buffer.size < vec->esize * vec->len) {
		fatptr_alloc_more(&vec->buffer, vec->esize * vec->len * vec_multiplier(vec->len));
	}
	memcpy(vec_at(vec, vec->len - 1), ptr, vec->esize);
}

void *vec_pop(Vec *vec) {
	assert(vec->len > 0 && "trying to pop empty array");
	void *result = vec_at(vec, vec->len - 1);
	vec->len--;
	return result;
}

void vec_free(Vec *vec) {
	fatptr_free(&vec->buffer);
}

void *vec_at(Vec *vec, size_t idx) {
	if (idx >= vec->len) {
		return NULL;
	}
	return &vec->buffer.str[idx * vec->esize];
}

void *vec_top(Vec *vec) {
	return vec_at(vec, vec->len - 1);
}
