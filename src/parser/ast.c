#include "ast.h"

void ast_print_type(AstType *type) {
	switch (type->type) {
		case AST_TYPE_IDENT:
			printf("%P", &type->ident);
			break;
	}
}

void ast_print_expr(Expr *expr) {
	switch (expr->type) {
		case EXPR_BINOP: {
			static char *binop_strs[] = {
				[BINOP_ADD] = "+",
				[BINOP_SUB] = "-",
				[BINOP_MUL] = "*",
				[BINOP_DIV] = "/",
				[BINOP_EQ]  = "==",
				[BINOP_NEQ] = "!=",
				[BINOP_GT]  = ">",
				[BINOP_GE]  = ">=",
				[BINOP_LT]  = "<",
				[BINOP_LE]  = "<=",
			};
			printf("(");
			ast_print_expr(expr->binop.left);
			printf("%s", binop_strs[expr->binop.type]);
			ast_print_expr(expr->binop.right);
			printf(")");
			break;
		}
/*		case EXPR_UNARY: {
			static char *unary_strs[] = {
				[UNARY_MINUS] = "-"
			};
			puts(unary_strs[expr->unary.type]);
			printf("(");
			ast_print_expr(expr->unary.expr);
			printf(")");
			break;
		}*/
		case EXPR_FUNCALL: {
			printf("%P(", &expr->funcall.name);
			bool first = true;
			foreach(&expr->funcall.args, Expr, arg) {
				if (!first) {
					printf(", ");
				} else {
					first = false;
				}
				ast_print_expr(arg);
			}
			printf(")");
			break;
		}
		case EXPR_IDENT:
			printf("%P", &expr->ident);
			break;
		case EXPR_INTEGER:
			printf("%ld", expr->integer);
			break;
		case EXPR_BOOL:
			printf("%s", expr->boolean ? "true" : "false");
			break;
	}
}

void ast_print_body(AstBody *body, size_t level);

void ast_print_if_body(AstIfBody *body, size_t level) {
	printf("if ");
	ast_print_expr(&body->expr);
	printf(" ");
	ast_print_body(&body->body, level);
}

void ast_print_stmt(AstStmt *stmt, size_t level) {
	for (size_t i = 0; i < level; i++) printf("\t");
	switch (stmt->type) {
		case AST_STMT_IF: {
			AstIf *if_else = &stmt->if_else;
			ast_print_if_body(&if_else->main, level);
			foreach(&if_else->else_ifs, AstIfBody, body) {
				printf(" else ");
				ast_print_if_body(&if_else->main, level);
			}
			if (if_else->has_else) {
				printf(" else ");
				ast_print_body(&if_else->else_body, level);
			}
			break;
		}
		case AST_STMT_FUNCALL: {
			Funcall *funcall = &stmt->funcall;
			printf("%P(", &funcall->name);
			bool first = true;
			foreach(&funcall->args, Expr, expr) {
				if (!first) {
					printf(", ");
				} else {
					first = false;
				}
				ast_print_expr(expr);
			}
			printf(");");
			break;
		}
		case AST_STMT_VAR: {
			AstVar *var = &stmt->var;
			printf("var %P", &var->name);
			if (var->typed) {
				printf(": ");
				ast_print_type(&var->type);
			}
			if (var->initializes) {
				printf(" = ");
				ast_print_expr(&var->expr);
			}
			printf(";");
			break;
		}
		case AST_STMT_RETURN: {
			AstReturn *ret = &stmt->ret;
			printf("return");
			if (ret->has_value) {
				printf(" ");
				ast_print_expr(&ret->expr);
			}
			printf(";");
			break;
		}
	}
	printf("\n");
}

void ast_print_body(AstBody *body, size_t level) {
	printf("{");
	if (body->stmts.len > 0) {
		printf("\n");
		foreach(&body->stmts, AstStmt, stmt) ast_print_stmt(stmt, level + 1);
		for (size_t i = 0; i < level; i++) printf("\t");
	}
	printf("}");
}

void ast_print_func_info(AstFuncInfo *info) {
	printf("fun %P(", &info->name);
	bool first = true;
	foreach(&info->args, AstFuncArg, arg) {
		if (first) {
			first = false;
		} else {
			printf(", ");
		}
		printf("%P: ", &arg->name);
		ast_print_type(&arg->type);
	}
	printf("): ");
	ast_print_type(&info->returning);
}

void ast_print_module_node(AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_FUNC: {
			AstFuncDecl *decl = &node->func_decl;
			ast_print_func_info(&decl->info);
			printf(" ");
			ast_print_body(&decl->body, 0);
			break;
		}
		case AST_MODULE_NODE_EXTERNAL_FUNC: {
			printf("extern ");
			ast_print_func_info(&node->ext_func_decl);
			break;
		}
	}
	printf("\n");
}

void ast_print_module(AstModule *module) {
	foreach (&module->nodes, AstModuleNode, node) {
		ast_print_module_node(node);
	}
}
