#include <stdarg.h>
#include "ast/private/type.h"
#include "ast/private/module_node.h"
#include "core/vec.h"
#include "core/print.h"

void print_ast_type(FILE *stream, va_list list) {
	AstType *type = va_arg(list, AstType*);
	switch (type->type) {
		case AST_TYPE_FUNCTION:
			print_to(stream, "fun (");
			for (size_t i = 0; i < vec_len(type->func.args); i++) {
				print_to(stream, "{ast::type}", &type->func.args[i]);
			}
			print_to(stream, "): {ast::type}", type->func.returns);
			break;
		case AST_TYPE_POINTER: print_to(stream, "*{ast::type}", type->ptr_to); break;
		case AST_TYPE_PATH: print_to(stream, "{ast::path}", &type->path); break;
		case AST_TYPE_SLICE: print_to(stream, "[]{ast::type}", type->slice_of); break;
		case AST_TYPE_ARRAY: print_to(stream, "[{long}]{ast::type}", type->array.length, type->array.of); break;
	}
}
