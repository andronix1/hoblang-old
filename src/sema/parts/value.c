#include "../parts.h"

SemaType *sema_ast_value(SemaModule *sema, AstValue *value) {
	SemaScopeValueDecl *decl = sema_resolve_mod_path_value(sema, &value->mod_path);
	if (!decl) {
		return NULL;
	}	
	SemaType *type = decl->type;
	value->sema_type = type;
	for (size_t i = 0; i < vec_len(value->segments); i++) {
		AstValueSegment *seg = &value->segments[i];
		switch (seg->type) {
			case AST_VALUE_IDENT:
				if (type->type != SEMA_TYPE_STRUCT) {
					sema_err("cannot get member {slice} of type {sema::type}", &seg->ident, type);
					return NULL;
				}
				bool found = false;
				for (size_t i = 0; i < vec_len(type->struct_type->members); i++) {
					AstStructMember *member = &type->struct_type->members[i];
					if (slice_eq(&member->name, &seg->ident.ident)) {
						type = member->type->sema;
						seg->ident.struct_member_idx = i;
						seg->ident.struct_sema_type = type;
						found = true;
						break;
					}
				}
				if (found) {
					break;
				}
				sema_err("member {slice} doesn't exists in given type", &seg->ident.ident);
				return NULL;

			case AST_VALUE_IDX: {
				if (type->type != SEMA_TYPE_SLICE) {
					sema_err("only slices can be indexed");
					return NULL;
				}
				type = type->slice_of;
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
