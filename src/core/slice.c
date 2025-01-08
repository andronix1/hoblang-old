#include "slice.h"

Slice slice_from_cstr(const char *cstr) {
	Slice result = {
		.len = strlen(cstr),
		.str = cstr
	};
	return result;
}

size_t slice_write_to(const Slice *slice, FILE *stream) {
	return fwrite(slice->str, 1, slice->len, stream);
}

bool slice_eq(const Slice *slice, const Slice *other) {
	if (slice->len != other->len) return false;
	return !memcmp(slice->str, other->str, slice->len);
}
