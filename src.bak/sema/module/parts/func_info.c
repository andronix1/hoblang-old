#include "sema/module/parts/func_info.h"
#include "sema/module/parts/type.h"

SemaType *sema_ast_func(SemaModule *sema, FileLocation at, SemaType *ext_of, AstFuncArg *args, AstType *returning) {
	SemaType *sema_returning = sema_ast_type(sema, returning);
	if (!returning) {
		return NULL;
	}

	SemaType **sema_args = vec_new(SemaType*);
	if (ext_of) {
		sema_args = vec_push(sema_args, &ext_of);
	}
	for (size_t i = 0; i < vec_len(args); i++) {
		AstFuncArg *arg = &args[i];
		SemaType *type = sema_ast_type(sema, arg->type);
		if (!type) {
			return NULL;
		}
		sema_args = vec_push(sema_args, &type);
	}
    return sema_type_new_func(sema_returning, sema_args);
}
