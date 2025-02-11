#include <string.h>
#include <errno.h>
#include "core/slice/io.h"
#include "print/base.h"

void print_slice(FILE *stream, va_list list) {
	slice_write_to(va_arg(list, Slice*), stream);
}

void print_errno(FILE *stream, va_list list __attribute__((unused))) {
	fprintf(stream, "%s", strerror(errno));
}

void print_bool(FILE *stream, va_list list) {
	fprintf(stream, "%s", va_arg(list, int) ? "true" : "false");
}

void print_size(FILE *stream, va_list list) {
	fprintf(stream, "%lu", va_arg(list, size_t));
}

void print_int(FILE *stream, va_list list) {
	fprintf(stream, "%d", va_arg(list, int));
}

void print_char(FILE *stream, va_list list) {
	fputc(va_arg(list, int), stream);
}

void print_cstr(FILE *stream, va_list list) {
	fputs(va_arg(list, const char*), stream);
}

