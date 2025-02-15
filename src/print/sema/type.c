#include "print.h"
#include "ast/private/type.h"
#include "sema/type/private.h"
#include "ast/private/module_node.h"
#include "core/vec.h"

void print_sema_type(FILE* stream, va_list list) {
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
			switch (type->primitive.type) {
				case SEMA_PRIMITIVE_BOOL: print_to(stream, "bool"); break;
				case SEMA_PRIMITIVE_VOID: print_to(stream, "void"); break;
				case SEMA_PRIMITIVE_INT: print_to(stream, (const char*[]){
					[SEMA_PRIMITIVE_INT8] = "i8",
					[SEMA_PRIMITIVE_INT16] = "i16",
					[SEMA_PRIMITIVE_INT32] = "i32",
					[SEMA_PRIMITIVE_INT64] = "i64",
					[SEMA_PRIMITIVE_UINT8] = "u8",
					[SEMA_PRIMITIVE_UINT16] = "u16",
					[SEMA_PRIMITIVE_UINT32] = "u32",
					[SEMA_PRIMITIVE_UINT64] = "u64"
				}[type->primitive.integer]); break;
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

