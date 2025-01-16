SemaType *sema_ast_expr_int(SemaModule *sema __attribute__((unused)), uint64_t integer __attribute__((unused)), SemaType *expectation) {
	if (expectation && expectation->type == SEMA_TYPE_PRIMITIVE && expectation->primitive != PRIMITIVE_VOID) {
		return expectation;
	} else {
		return &primitives[PRIMITIVE_I32];
	}
}
