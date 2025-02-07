#include "../parts.h"

SemaType *sema_ast_type(SemaModule *sema, AstType *type) {
	if (type->sema) {
		return type->sema;
	}
	switch (type->type) {
		case AST_TYPE_FUNCTION: {
			SemaType **args = vec_new(SemaType*);
			for (size_t i = 0; i < vec_len(type->func.args); i++) {
				SemaType* arg_type = sema_ast_type(sema, &type->func.args[i]);
				if (!arg_type) {
					return NULL;
				}
				args = vec_push(args, &arg_type);
			}
			SemaType *stype = malloc(sizeof(SemaType));
			stype->type = SEMA_TYPE_FUNCTION;
			stype->func.args = args;
			stype->func.returning = sema_ast_type(sema, type->func.returns);
			type->sema = stype;
			break;
		}
		case AST_TYPE_STRUCT: {
			for (size_t i = 0; i < vec_len(type->struct_type.members); i++) {
				AstStructMember *member = &type->struct_type.members[i];
				for (size_t j = 0; j < i; j++) {
					AstStructMember *jmember = &type->struct_type.members[j];
					if (slice_eq(&member->name, &jmember->name)) {
						sema_err("field {slice} duplicated", &member->name);
					}
				}
				sema_ast_type(sema, member->type);
			}
			SemaType *stype = malloc(sizeof(SemaType));
			stype->type = SEMA_TYPE_STRUCT;
			stype->struct_type = &type->struct_type;
			type->sema = stype;
			break;
		}
		case AST_TYPE_PATH: {
			type->sema = sema_resolve_type_path(sema, &type->path);
			break;
		}
		case AST_TYPE_ARRAY: {
			SemaType *array_of = sema_ast_type(sema, type->array.of);
			if (!array_of) {
				return NULL;
			}
			type->sema = sema_type_new_array(sema_eval_int_expr(sema, type->array.length), array_of);
			break;
		}
		case AST_TYPE_SLICE: {
			SemaType *slice_of = sema_ast_type(sema, type->slice_of);
			if (!slice_of) {
				return NULL;
			}
			type->sema = sema_type_new_slice(slice_of);
			break;
		}
		case AST_TYPE_POINTER: {
			SemaType *ptr_to = sema_ast_type(sema, type->ptr_to);
			if (!ptr_to) {
				return NULL;
			}
			type->sema = sema_type_new_pointer(ptr_to);
			break;
		}
	}
	return type->sema;
}
