#pragma once

#include <string.h>
#include <malloc.h>

typedef struct {
	size_t len, cap, esize;
} VecHeader;

VecHeader *vec_header(void *vec);
void *vec_new_sized(size_t esize);
void *vec_reserve(void *vec, size_t cap);
void *vec_push(void *vec, void *element);
size_t vec_len(void *vec);
void vec_free(void *vec);

#define vec_new(type) ((type*)vec_new_sized(sizeof(type)))
