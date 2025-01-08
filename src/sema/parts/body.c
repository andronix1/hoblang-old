#include "../parts.h"

void sema_check_if_else_body(Sema *sema, AstIfBody *if_body, SemaType *returning) {
	SemaType type;
	if (sema_ast_expr_type(sema, &type, &if_body->expr, &primitives[PRIMITIVE_BOOL])) {
		if (!sema_types_equals(&type, &primitives[PRIMITIVE_BOOL])) {
			sema_err("if condition expression must be boolean, not {sema::type}", &type);
		}
	}
	sema_ast_body(sema, if_body->body, returning);
}

void sema_ast_body(Sema *sema, AstBody *body, SemaType *returning) {
	for (size_t i = 0; i < vec_len(body->stmts); i++) {
		AstStmt *stmt = &body->stmts[i];
		switch (stmt->type) {
			case AST_STMT_VAR: {
				SemaRecord decl = {
					.name = stmt->var.name,
				};
				if (stmt->var.typed) {
					if (!sema_ast_type(sema, &decl.type, &stmt->var.type)) {
						break;
					}
					if (stmt->var.initializes) {
						SemaType type;
						if (!sema_ast_expr_type(sema, &type, &stmt->var.expr, &decl.type)) {
							break;
						}
						if (!sema_types_equals(&type, &decl.type)) {
							sema_err("cannot put {sema::type} in {sema::type} variable", &type, &decl.type);
							break;
						}
					}
				} else {
					if (stmt->var.initializes) {
						if (!sema_ast_expr_type(sema, &decl.type, &stmt->var.expr, NULL)) {
							break;
						}
					} else {
						sema_err("variable type must be specified or initializer must present");
						break;
					}
				}
				sema_push_decl(sema, &decl);
				break;
			}
			case AST_STMT_RETURN: {
				if (stmt->ret.has_value) {
					SemaType type;
					if (!sema_ast_expr_type(sema, &type, &stmt->ret.expr, returning)) {
						break;
					}
					if (!sema_types_equals(returning, &type)) {
						sema_err("expected to return `{sema::type}` got `{sema::type}`", returning, &type);
						break;
					}
				} else {
					if (!sema_types_equals(returning, &primitives[PRIMITIVE_VOID])) {
						sema_err("expected to return `{sema::type}` but got nothing");
						break;
					}
				}
				break;
			}
			case AST_STMT_IF: {	
				sema_check_if_else_body(sema, &stmt->if_else.main, returning);
				for (size_t i = 0; i < vec_len(stmt->if_else.else_ifs); i++) {
					sema_check_if_else_body(sema, &stmt->if_else.else_ifs[i], returning);
				}
				if (stmt->if_else.has_else) {
					sema_ast_body(sema, stmt->if_else.else_body, returning);
				}
				break;
			}
			case AST_STMT_FUNC_CALL: {		
				sema_ast_func_call(sema, NULL, &stmt->func_call);
				break;
			}
		}
	}
} 
