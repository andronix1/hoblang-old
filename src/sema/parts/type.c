#include "../parts.h"

bool sema_ast_type(Sema *sema, SemaType *output, AstType *type) {
	if (type->sema) {
		if (output) {
			memcpy(output, type->sema, sizeof(SemaType));
		}
		return type->sema;
	}
	SemaType *sema_type = sema_resolve_ident_type(sema, &type->ident);
	if (!sema_type) {
		sema_err("unknown type `{slice}`", &type->ident);
		return false;
	}
	type->sema = sema_type;
	if (output) {
		memcpy(output, type->sema, sizeof(SemaType));
	}
	return true;
}
