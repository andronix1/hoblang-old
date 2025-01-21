#include <stdarg.h>
#include "../ast.h"

void print_ast_type(FILE *stream, va_list *list) {
	AstType *type = va_arg(*list, AstType*);
	switch (type->type) {
		case AST_TYPE_POINTER: print_to(stream, "*{ast::type}", type->ptr_to); break;
		case AST_TYPE_PATH: print_to(stream, "<mod path is NIY>", type->ptr_to); break;
	}
}
