#include "../parts.h"

SemaType *sema_ast_type(SemaModule *sema, AstType *type) {
	if (type->sema) {
		return type->sema;
	}
	switch (type->type) {
		case AST_TYPE_IDENT: {
			SemaType *sema_type = sema_resolve_ident_type(sema, &type->ident);
			if (!sema_type) {
				sema_err("unknown type `{slice}`", &type->ident);
				return NULL;
			}
			type->sema = sema_type;
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
