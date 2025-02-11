#include "core/slice/api.h"
#include <string.h>
#include <malloc.h>

Slice slice_from_cstr(const char *cstr) {
	Slice result = {
		.len = strlen(cstr),
		.str = cstr
	};
	return result;
}

Slice slice_new(const char *str, size_t len) {
	Slice result = {
		.str = str,
		.len = len
	};
	return result;
}

char *slice_to_cstr(const Slice *slice) {
	char *result = malloc(slice->len + 1);
	memcpy(result, slice->str, slice->len);
	result[slice->len] = 0;
	return result;
}

bool slice_eq(const Slice *slice, const Slice *other) {
	if (slice->len != other->len) return false;
	return !memcmp(slice->str, other->str, slice->len);
}
