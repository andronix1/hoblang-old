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
				if (type->type == SEMA_TYPE_SLICE) {
					Slice length = slice_from_cstr("length");
					Slice raw = slice_from_cstr("raw");
					seg->ident.get_type = SEMA_VALUE_GET_SLICE;
					if (slice_eq(&seg->ident.ident, &length)) {
						seg->ident.slice_ident.member = SEMA_VALUE_SLICE_MEMBER_LENGTH;
						type = &primitives[PRIMITIVE_I32];
					} else if (slice_eq(&seg->ident.ident, &raw)) {
						seg->ident.slice_ident.member = SEMA_VALUE_SLICE_MEMBER_POINTER;
						seg->ident.slice_ident.slice_of = type->slice_of;
						type = sema_type_new_pointer(type->slice_of);
					} else {
						sema_err("cannot get member {slice} of type {sema::type}", &seg->ident, type);
						return NULL;
					}
					break;
				} else if (type->type == SEMA_TYPE_STRUCT) {
					bool found = false;
					for (size_t i = 0; i < vec_len(type->struct_type->members); i++) {
						AstStructMember *member = &type->struct_type->members[i];
						if (slice_eq(&member->name, &seg->ident.ident)) {
							type = member->type->sema;
							seg->ident.get_type = SEMA_VALUE_GET_STRUCT;
							seg->ident.struct_ident.member_idx = i;
							seg->ident.struct_ident.sema_type = type;
							found = true;
							break;
						}
					}
					if (found) {
						break;
					}
					sema_err("member {slice} doesn't exists in given type", &seg->ident.ident);
					return NULL;
				} else {
					sema_err("cannot get member {slice} of type {sema::type}", &seg->ident, type);
					return NULL;
				}

			case AST_VALUE_IDX: {
				if (type->type == SEMA_TYPE_SLICE) {
					seg->idx.type = SEMA_VALUE_IDX_SLICE;
				} else if (type->type == SEMA_TYPE_ARRAY) {
					seg->idx.type = SEMA_VALUE_IDX_ARRAY;
				} else {
					sema_err("only slices or arrays can be indexed");
					return NULL;
				}
				SemaType *idx_type = sema_ast_expr_type(sema, seg->idx.expr, &primitives[PRIMITIVE_I32]);
				if (idx_type && !sema_types_equals(idx_type, &primitives[PRIMITIVE_I32])) {
					sema_err("indexes must be i32");
				}
				type = type->slice_of;
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
