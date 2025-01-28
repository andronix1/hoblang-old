#pragma once

#include <string.h>
#include <malloc.h>
#include "assert.h"

typedef struct {
	size_t len, cap, esize;
} VecHeader;

VecHeader *vec_header(void *vec);
void *vec_new_sized(size_t esize);
void *vec_reserve(void *vec, size_t cap);
void *vec_push(void *vec, void *element);
void *vec_top(void *vec);
void *vec_pop(void *vec);
size_t vec_len(void *vec);
void *_vec_append_raw(void *vec, const void *ptr, size_t len);
void vec_free(void *vec);

#define vec_new(type) ((type*)_vec_new(sizeof(type)))

#define vec_top(vec) (&vec[vec_len(vec) - 1])
#define vec_pop(vec) ((typeof(vec))_vec_pop(vec))

#define vec_reserve(vec, cap) ((typeof(vec))_vec_reserve(vec, cap))

#define vec_push(vec, element) ({ \
	const typeof(vec) _element = (element); \
	assert(sizeof(*_element) == vec_header(vec)->esize, "trying to push value with size {long} to vec with esize {long}", sizeof(*_element), vec_header(vec)->esize); \
	(typeof(vec))_vec_push((vec), _element); \
})

#define vec_append_raw(vec, ptr, len) ({ \
	assert(sizeof(*ptr) == vec_header(vec)->esize, "trying to append vector with esize {long} by pointer with size {long}", sizeof(*ptr), vec_header(vec)->esize); \
	(typeof(vec))_vec_append_raw((vec), ptr, len); \
})
