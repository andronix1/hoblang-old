#include "../parts.h"

SemaType *sema_ast_value(Sema *sema, AstValue *value) {
	assert(vec_len(value->segments) > 0, "no segments in value");
	SemaType *type = sema_resolve_decl_type(sema, &value->segments[0].ident);
	if (!type) {
		return NULL;
	}
	value->sema_type = value->segments[0].sema_type = type;
	for (size_t i = 1; i < vec_len(value->segments); i++) {
		AstValueSegment *seg = &value->segments[i];
		switch (seg->type) {
			case AST_VALUE_IDENT:
				sema_err("inner idents are not supported now");
				return NULL;

			case AST_VALUE_REF:
				type = sema_type_new_pointer(type);
				break;

			case AST_VALUE_DEREF:
				if (type->type != SEMA_TYPE_POINTER) {
					sema_err("only pointers can be dereferenced");
					return NULL;
				}
				type = type->ptr_to;
				break;
		}
		value->sema_type = seg->sema_type = type;
	}
	return type;
}
