#include "core/print.h"
#include "print/base.h"
#include "print/ast.h"
#include "print/lexer.h"
#include "print/sema.h"
#include "core/slice/api.h"

void print_setup() {
	print_init();
	print_register(slice_from_cstr("cstr"), print_cstr);
	print_register(slice_from_cstr("char"), print_char);
	print_register(slice_from_cstr("int"), print_int);
	print_register(slice_from_cstr("bool"), print_bool);
	print_register(slice_from_cstr("long"), print_size);
	print_register(slice_from_cstr("size"), print_size);
	print_register(slice_from_cstr("errno"), print_errno);
	print_register(slice_from_cstr("tok"), print_token);
	print_register(slice_from_cstr("slice"), print_slice);
    print_register(slice_from_cstr("sema::type"), print_sema_type);
	print_register(slice_from_cstr("ast::binop"), print_ast_binop_type);
	print_register(slice_from_cstr("ast::path"), print_ast_path);
	print_register(slice_from_cstr("ast::type"), print_ast_type);
}
