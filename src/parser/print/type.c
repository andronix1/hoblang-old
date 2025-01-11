#include <stdarg.h>
#include "../ast.h"

void print_ast_type(FILE *stream, va_list *list) {
	AstType *type = va_arg(*list, AstType*);
	print_to(stream, "{slice}", &type->ident);
}
