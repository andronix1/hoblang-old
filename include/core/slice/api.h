#pragma once

#include <stdbool.h>
#include "slice.h"

Slice slice_from_cstr(const char *cstr);
Slice slice_new(const char *ptr, size_t len);
char *slice_to_cstr(const Slice *slice);
bool slice_eq(const Slice *slice, const Slice *other);

#define slice_from_const_cstr(cstr) { .str = cstr, .len = sizeof(cstr) - 1 }
