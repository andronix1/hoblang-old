#pragma once

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

typedef struct {
	size_t len;
	const char *str;
} Slice;

Slice slice_from_cstr(const char *cstr);
size_t slice_write_to(const Slice *slice, FILE *stream);
bool slice_eq(const Slice *slice, const Slice *other);
