#include "type.h"

Type primitives[] = {
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

bool types_equals(Type *type, Type *other) {
	if (type->type != other->type) {
		return false;
	}
	switch (type->type) {
		case TYPE_PRIMITIVE:
			return type->primitive == other->primitive;
		case TYPE_FUNCTION:
			if (type->func.args.len != other->func.args.len) {
				return false;
			}
			for (size_t i = 0; i < type->func.args.len; i++) {
				TypeFuncArg *targ = vec_at(&type->func.args, i);
				TypeFuncArg *oarg = vec_at(&other->func.args, i);
				if (!types_equals(targ->type, oarg->type)) {
					return false;
				}
			}
			return types_equals(type->func.returning, other->func.returning);
	}
	assert(0 && "unknown type kind");
	return false;
}
