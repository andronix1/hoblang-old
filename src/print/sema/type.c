#include "print.h"
#include "ast/private/type.h"
#include "sema/type/private.h"
#include "ast/private/module_node.h"
#include "core/vec.h"

void print_sema_type(FILE* stream, va_list list) {
	const char *strs[] = {
		[PRIMITIVE_I8] = "i8",
		[PRIMITIVE_I16] = "i16",
		[PRIMITIVE_I32] = "i32",
		[PRIMITIVE_I64] = "i64",
		[PRIMITIVE_U8] = "u8",
		[PRIMITIVE_U16] = "u16",
		[PRIMITIVE_U32] = "u32",
		[PRIMITIVE_U64] = "u64",
		[PRIMITIVE_BOOL] = "bool",
		[PRIMITIVE_VOID] = "void",
	};
	SemaType *type = va_arg(list, SemaType*);
	switch (type->type) {
		case SEMA_TYPE_STRUCT:
			print_to(stream, "struct {slice} ", &type->struct_def->name);
			fprintf(stream, "{ ");

			for (size_t i = 0; i < vec_len(type->struct_def->members); i++) {
				if (i != 0) {
					print_to(stream, ", ");
				}
				AstStructMember *member = &type->struct_def->members[i];
				print_to(stream, "{slice}: {sema::type}", &member->name, member->type->sema);
			}
			print_to(stream, " }");
			break;
		case SEMA_TYPE_PRIMITIVE:
			if (type->primitive >= (sizeof(strs) / sizeof(strs[0]))) {
				print_to(stream, "<unknown {int}>", type->primitive);
			} else {
				print_to(stream, strs[type->primitive]);
			}
			break;
		case SEMA_TYPE_FUNCTION:
			print_to(stream, "fun (");
			for (size_t i = 0; i < vec_len(type->func.args); i++) {
				if (i != 0) print_to(stream, ", ");
				print_to(stream, "{sema::type}", type->func.args[i]);
			}
			print_to(stream, "): {sema::type}", type->func.returning);
			break;
		case SEMA_TYPE_SLICE:
			print_to(stream, "[]{sema::type}", type->slice_of);
			break;
		case SEMA_TYPE_ARRAY:
			print_to(stream, "[{long}]", type->array.length);
			print_to(stream, "{sema::type}", type->array.of);
			break;
		case SEMA_TYPE_POINTER:
			print_to(stream, "*{sema::type}", type->ptr_to);
			break;
	}
}

