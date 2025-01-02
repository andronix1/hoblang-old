#include "expr.h"

#define expr_free_opt(expr) if (expr) { expr_free(expr); free(expr); }

void expr_free(Expr *expr) {
	if (expr->type == EXPR_BINOP) {
		expr_free_opt(expr->binop.left);
		expr_free_opt(expr->binop.right);
	} else if (expr->type == EXPR_UNARY) {
		expr_free_opt(expr->unary.expr);
	}
}
