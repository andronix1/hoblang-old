#pragma once

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>

typedef struct {
	size_t len;
	const char *str;
} Slice;

Slice slice_from_cstr(const char *cstr);
Slice slice_new(const char *ptr, size_t len);
size_t slice_write_to(const Slice *slice, FILE *stream);
char *slice_to_cstr(const Slice *slice);
bool slice_eq(const Slice *slice, const Slice *other);

#define slice_from_const_cstr(cstr) { .str = cstr, .len = sizeof(cstr) - 1 }