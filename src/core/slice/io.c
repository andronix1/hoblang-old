#include "core/slice/io.h"
#include <malloc.h>

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

size_t slice_write_to(const Slice *slice, FILE *stream) {
	return fwrite(slice->str, 1, slice->len, stream);
}
