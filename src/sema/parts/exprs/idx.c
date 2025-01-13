SemaType *sema_ast_expr_idx(Sema *sema, AstIdx *idx, SemaType *expectation) {
	SemaType *expr_type = sema_ast_expr_type(sema, idx->expr, expectation == NULL ? NULL : sema_type_new_pointer(expectation));
	if (!expr_type) {
		return NULL;
	}
	SemaType *idx_type = sema_ast_expr_type(sema, idx->idx, &primitives[PRIMITIVE_I32]);
	if (!idx_type) {
		return NULL;
	}
	if (!sema_types_equals(idx_type, &primitives[PRIMITIVE_I32])) {
		sema_err("slice idx must be i32, not {sema::type}", &idx_type);
	}
	return expr_type->ptr_to;
}
