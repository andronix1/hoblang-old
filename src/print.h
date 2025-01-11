#include <string.h>
#include <errno.h>
#include "parser/ast.h"
#include "parser/print.h"
#include "lexer/tokens.h"
#include "core/print.h"
#include "sema/type.h"

void print_slice(FILE *stream, va_list *list) {
	slice_write_to(va_arg(*list, Slice*), stream);
}

void print_errno(FILE *stream, va_list *list __attribute__((unused))) {
	fprintf(stream, "%s", strerror(errno));
}

void print_bool(FILE *stream, va_list *list) {
	fprintf(stream, "%s", va_arg(*list, int) ? "true" : "false");
}

void print_size(FILE *stream, va_list *list) {
	fprintf(stream, "%lu", va_arg(*list, size_t));
}

void print_int(FILE *stream, va_list *list) {
	fprintf(stream, "%d", va_arg(*list, int));
}

void print_cstr(FILE *stream, va_list *list) {
	fputs(va_arg(*list, const char*), stream);
}

void print_setup() {
	print_init();
	print_register(slice_from_cstr("cstr"), print_cstr);
	print_register(slice_from_cstr("int"), print_int);
	print_register(slice_from_cstr("bool"), print_bool);
	print_register(slice_from_cstr("long"), print_size);
	print_register(slice_from_cstr("size"), print_size);
	print_register(slice_from_cstr("errno"), print_errno);
	print_register(slice_from_cstr("tok"), print_token);
	print_register(slice_from_cstr("slice"), print_slice);
	print_register(slice_from_cstr("sema::type"), print_sema_type);
	print_register(slice_from_cstr("ast::expr"), print_ast_expr);
	print_register(slice_from_cstr("ast::binop"), print_ast_binop_type);
	print_register(slice_from_cstr("ast::type"), print_ast_type);
}
