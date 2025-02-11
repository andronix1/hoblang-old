#include "api.h"
#include "core/vec.h"
#include "private.h"

bool sema_types_equals(SemaType *type, SemaType *other) {
	if (type == other) {
		return true;
	}
	if (type->type != other->type) {
		return false;
	}
	switch (type->type) {
		case SEMA_TYPE_STRUCT:
			if (vec_len(type->struct_type->members) != vec_len(other->struct_type->members)) {
				return false;
			}
			for (size_t i = 0; i < vec_len(type->struct_type->members); i++) {
				AstStructMember *a = &type->struct_type->members[i],
								*b = &other->struct_type->members[i];
				if (!slice_eq(&a->name, &b->name) || !sema_types_equals(a->type->sema, b->type->sema)) {
					return false;
				}
			}
			return true;
		case SEMA_TYPE_PRIMITIVE:
			return type->primitive == other->primitive;
		case SEMA_TYPE_SLICE:
			return sema_types_equals(type->slice_of, other->slice_of);
		case SEMA_TYPE_ARRAY:
			return 
				type->array.length == other->array.length &&
				sema_types_equals(type->array.of, other->array.of);
		case SEMA_TYPE_FUNCTION:
			if (vec_len(type->func.args) != vec_len(other->func.args)) {
				return false;
			}
			if (!sema_types_equals(type->func.returning, other->func.returning)) {
				return false;
			}
			for (size_t i = 0; i < vec_len(type->func.args); i++) {	
				if (!sema_types_equals(type->func.args[i], other->func.args[i])) {
					return false;
				}
			}
			return true;
		case SEMA_TYPE_POINTER:
			return sema_types_equals(type->ptr_to, other->ptr_to);
	}
	assert(0, "invalid sema type kind {int}", type->type);
}

bool sema_type_is_primitive(SemaType *type, Primitive primitive) {
    return type->type == SEMA_TYPE_PRIMITIVE && type->primitive == primitive;
}
