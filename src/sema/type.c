#include "type.h"

#define TYPE_PRIMITIVE(kind) [kind] = { .type = SEMA_TYPE_PRIMITIVE, .primitive = kind }

SemaType primitives[] = {
	TYPE_PRIMITIVE(PRIMITIVE_U8),
	TYPE_PRIMITIVE(PRIMITIVE_U16),
	TYPE_PRIMITIVE(PRIMITIVE_U32),
	TYPE_PRIMITIVE(PRIMITIVE_U64),
	TYPE_PRIMITIVE(PRIMITIVE_I8),
	TYPE_PRIMITIVE(PRIMITIVE_I16),
	TYPE_PRIMITIVE(PRIMITIVE_I32),
	TYPE_PRIMITIVE(PRIMITIVE_I64),
	TYPE_PRIMITIVE(PRIMITIVE_BOOL),
	TYPE_PRIMITIVE(PRIMITIVE_VOID),
};

bool sema_types_equals(SemaType *type, SemaType *other) {
	if (type->type != other->type) {
		return false;
	}
	switch (type->type) {
		case SEMA_TYPE_PRIMITIVE:
			return type->primitive == other->primitive;
		case SEMA_TYPE_FUNCTION:
			if (vec_len(type->func.args) != vec_len(other->func.args)) {
				return false;
			}
			if (!sema_types_equals(type->func.returning, other->func.returning)) {
				return false;
			}
			for (size_t i = 0; i < vec_len(type->func.args); i++) {	
				if (!sema_types_equals(type->func.args[i].type.sema, other->func.args[i].type.sema)) {
					return false;
				}
			}
			return true;
	}
	assert(0, "invalid sema type kind {int}", type->type);
}

void print_sema_type(FILE* stream, va_list *list) {
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
	SemaType *type = va_arg(*list, SemaType*);
	switch (type->type) {
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
				print_to(stream, "{sema::type}", type->func.args[i].type.sema);
			}
			print_to(stream, "): {sema::type}", type->func.returning);
			break;
	}
}
