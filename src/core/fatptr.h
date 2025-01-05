#pragma once

#include <stddef.h>
#include <malloc.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <printf.h>
#include "log.h"

typedef struct {
    size_t size;
    union {
        void *ptr;
        char *str;
    };
} FatPtr;

FatPtr fatptr_empty();
const char *fatptr_to_cstr(FatPtr *fatptr);
FatPtr fatptr_from_cstr(const char *cstr);
void fatptr_append(FatPtr *ptr, FatPtr *other);
bool fatptr_eq(FatPtr *ptr, FatPtr *other);
void fatptr_alloc_more(FatPtr *fatptr, size_t size);
size_t fatptr_print_to(const FatPtr *fatptr, FILE *stream);
void fatptr_print(FatPtr *fatptr);
bool fatptr_read_all(FatPtr *fatptr, FILE *file);
void fatptr_free(FatPtr *fatptr);
void fatptr_register_printf();
