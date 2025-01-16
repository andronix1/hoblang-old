#include "../parts.h"

SemaType *sema_ast_value(SemaModule *sema, AstValue *value) {
	assert(vec_len(value->segments) > 0, "no segments in value");
	value->decl = sema_resolve_value_decl(sema, &value->segments[0].ident);
	if (!value->decl) {
		sema_err("{slice} is undeclared!", &value->segments[0].ident);
		return NULL;
	}	
	SemaType *type = value->decl->type;
	value->sema_type = value->segments[0].sema_type = type;
	for (size_t i = 1; i < vec_len(value->segments); i++) {
		AstValueSegment *seg = &value->segments[i];
		switch (seg->type) {
			case AST_VALUE_IDENT:
				sema_err("inner idents are not supported now");
				return NULL;

			case AST_VALUE_IDX: {
				if (type->type != SEMA_TYPE_POINTER) {
					sema_err("only pointers can be indexed");
					return NULL;
				}
				type = type->ptr_to;
				SemaType *idx_type = sema_ast_expr_type(sema, seg->idx, &primitives[PRIMITIVE_I32]);
				if (idx_type && !sema_types_equals(idx_type, &primitives[PRIMITIVE_I32])) {
					sema_err("indexes must be i32");
				}
				break;
			}

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
