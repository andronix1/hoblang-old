#pragma once

#include "fatptr.h"

typedef struct {
	FatPtr buffer;
	size_t len, esize;
} Vec;

Vec vec_new_sized(size_t esize);
void vec_push(Vec *vec, void *ptr);
void vec_free(Vec *vec);
void *vec_at(Vec *vec, size_t idx);

#define foreach(vec, type, name) for (type *name = (vec)->buffer.ptr; (size_t)name < ((size_t)(vec)->buffer.ptr + (size_t)((vec)->len * (vec)->esize)); name = &name[1])
#define vec_new(type) vec_new_sized(sizeof(type))
