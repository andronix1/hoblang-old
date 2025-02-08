#include "slice.h"

bool slice_read_from_file(Slice *output, const char *path) {
	FILE *file = fopen(path, "r");
	if (!file) {
		return false;
	}
	fseek(file, 0, SEEK_END);
	size_t len = ftell(file);
	fseek(file, 0, SEEK_SET);
	output->str = malloc(len);
	if (fread((char*)output->str, 1, len, file) != len) {
		free((char*)output->str);
		fclose(file);
		return false;
	}
	output->len = len;
	return true;
}

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

size_t slice_write_to(const Slice *slice, FILE *stream) {
	return fwrite(slice->str, 1, slice->len, stream);
}

bool slice_eq(const Slice *slice, const Slice *other) {
	if (slice->len != other->len) return false;
	return !memcmp(slice->str, other->str, slice->len);
}
