#pragma once

#include <stddef.h>
#include <malloc.h>
#include <assert.h>

typedef struct {
    size_t size;
    union {
        void *raw_ptr;
        char *str_ptr;
    };
} FatPtr;

FatPtr fatptr_empty();
void fatptr_alloc_more(FatPtr *fatptr, size_t size);
void fatptr_free(FatPtr *fatptr);