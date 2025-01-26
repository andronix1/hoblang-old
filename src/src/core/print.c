#include "print.h"
#include "vec.h"

PrintInfo *print_infos;

void print_init() {
	print_infos = vec_new(PrintInfo);
}

void print_register(Slice name, PrintFunc func) {
	PrintInfo info = {
		.name = name,
		.func = func
	};
	print_infos = vec_push(print_infos, &info);
}

void print_va_list_to(FILE *stream, const char *fmt, va_list args) {
	Slice arg;
	bool is_arg = false;
	for (char c; (c = *fmt) != '\0'; fmt++) {
		if (is_arg) {
			if (c == '}') {
				is_arg = false;
				bool found = false;
				for (size_t i = 0; i < vec_len(print_infos); i++) {
					PrintInfo *info = &print_infos[i];
					if (slice_eq(&info->name, &arg)) {
						info->func(stream, args);
						found = true;
						break;
					}
				}
				if (!found) {
					fprintf(stream, "?");
					slice_write_to(&arg, stream);
					fprintf(stream, "?");
				}
			} else {
				arg.len++;
			}
		} else if (c == '{') {
			is_arg = true;
			arg.len = 0;
			arg.str = fmt + 1;
		} else {
			putc(c, stream);
		}
	}
}

void print_va_list(const char *fmt, va_list args) {
	print_va_list_to(stdout, fmt, args);
}

void print_to(FILE *stream, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	print_va_list_to(stream, fmt, args);
	va_end(args);
}

void print(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	print_va_list(fmt, args);
	va_end(args);
}
