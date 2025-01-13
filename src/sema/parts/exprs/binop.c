SemaType *sema_ast_expr_binop(Sema *sema, AstExprBinop *binop, SemaType *expectation) {
	bool binary_binop = (
			binop->type == AST_BINOP_EQ ||
			binop->type == AST_BINOP_NEQ ||
			binop->type == AST_BINOP_GT ||
			binop->type == AST_BINOP_GE ||
			binop->type == AST_BINOP_LT || 
			binop->type == AST_BINOP_LE
		);
	SemaType *expect = binary_binop ? NULL : expectation;
	SemaType *ltype = sema_ast_expr_type(sema, binop->left, expect);
	if (!ltype) {
		return NULL;
	}
	if (ltype->type != SEMA_TYPE_PRIMITIVE || ltype->primitive == PRIMITIVE_VOID) {
		sema_err("cannot use binop for type {sema::type}", ltype);
		return NULL;
	}
	if (binary_binop) {
		expect = ltype;
	}
	SemaType *right_type = sema_ast_expr_type(sema, binop->right, expect);
	if (!right_type) {
		return NULL;
	}
	if (!sema_types_equals(ltype, right_type)) {
		sema_err("cannot use binop {ast::binop} for types {sema::type} and {sema::type}", binop->type, ltype, &right_type);
		return NULL;
	}
	if (binary_binop) {
		ltype = &primitives[PRIMITIVE_BOOL];
	}
	return ltype;
}
