#include <stdarg.h>
#include "../ast.h"

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
		case AST_TYPE_PATH: print_to(stream, "<mod path is NIY>"); break;
		case AST_TYPE_SLICE: print_to(stream, "[]{ast::type}"); break;
		case AST_TYPE_STRUCT:
			print_to(stream, "struct {");
			for (size_t i = 0; i < vec_len(type->struct_type.members); i++) {
				if (i != 0) {
					print_to(stream, ", ");
				}
				AstStructMember *member = &type->struct_type.members[i];
				print_to(stream, " {slice}: {ast::type}", member->name, member->type);
			}
			print_to(stream, " }");
			break;
	}
}
