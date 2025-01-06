#include "sema.h"

Decl *sema_resolve_decl(Sema *sema, FatPtr *name) {
	foreach (&sema->scopes, Scope, scope) {
		foreach (&scope->decls, Decl, decl) {
			if (fatptr_eq(name, &decl->name)) {
				return decl;
			}
		}
	}
	return NULL;
}

bool sema_push_decl(Sema *sema, Decl *decl) {
	if (sema_resolve_decl(sema, &decl->name)) {
		sema_err("multiple definitions of `%P`", &decl->name);
		return false;
	}
	Scope *scope = vec_top(&sema->scopes);
	assert(scope && "scope stack is empty");
	vec_push(&scope->decls, decl);
	return true;
}

bool sema_resolve_ident_type(Sema *sema, Type *type, FatPtr *name) {
	foreach (&sema->scopes, Scope, scope) {
		foreach (&scope->types, ScopeType, ctype) {
			if (fatptr_eq(name, &ctype->name)) {
				*type = ctype->type;
				return true;
			}
		}
	}
	return false;
}

bool sema_ast_type(Sema *sema, Type *type, AstType *ast_type) {
	switch (ast_type->type) {
		case AST_TYPE_IDENT:
			if (!sema_resolve_ident_type(sema, type, &ast_type->ident)) {
				sema_err("unknown type `%P`", &ast_type->ident);
				return false;
			}
			ast_type->sema = *type;
			return true;
		default:
			assert(0 && "unknown ast type kind");
			return false;
	}
}

bool sema_binop_allowed(BinopType binop, Type *type) {
	switch (type->type) {
		case TYPE_PRIMITIVE:
			switch (type->primitive) {
				case PRIMITIVE_I8:
				case PRIMITIVE_I16:
				case PRIMITIVE_I32:
				case PRIMITIVE_I64:
				case PRIMITIVE_U8:
				case PRIMITIVE_U16:
				case PRIMITIVE_U32:
				case PRIMITIVE_U64:
					return true;
				case PRIMITIVE_BOOL:
					return binop == BINOP_EQ || binop == BINOP_NEQ;
				case PRIMITIVE_VOID:
					return false;
			}
			assert(0 && "unknown primitive");
			return false;
		case TYPE_FUNCTION:
			return false;
	}
	assert(0 && "unknown type kind");
	return false;
}

Type *sema_expr_resolve_type(Sema *sema, Expr *expr);

Type *sema_resolve_funcall_type(Sema *sema, Funcall *funcall) {
	Decl *decl = sema_resolve_decl(sema, &funcall->name);
	if (!decl) {
		sema_err("function `%P` is undefined", &funcall->name);
		return NULL;
	}
	if (decl->type.type != TYPE_FUNCTION) {
		sema_err("`%P` is not a function");
		return NULL;
	}
	if (decl->type.func.args.len != funcall->args.len) {
		sema_err("function `%P` takes %d arguments but %d were given", &funcall->name, decl->type.func.args.len, funcall->args.len);
		return decl->type.func.returning;
	}
	for (size_t i = 0; i < funcall->args.len; i++) {
		Type *passed = sema_expr_resolve_type(sema, vec_at(&funcall->args, i));
		if (!passed) {
			continue;
		}
		TypeFuncArg *arg = vec_at(&decl->type.func.args, i);
		if (!types_equals(passed, arg->type)) {
			sema_err("invalid type passed for argument `%P`", &arg->name);
		}
	}
	return decl->type.func.returning;
}

Type *sema_expr_resolve_type(Sema *sema, Expr *expr) {
	switch (expr->type) {
		case EXPR_IDENT: {
			Decl *decl = sema_resolve_decl(sema, &expr->ident);
			if (!decl) {
				sema_err("symbol `%P` undeclared", &expr->ident);
				return NULL;
			}
			expr->sema_type = decl->type;
			return &expr->sema_type;
		}
		case EXPR_AS: {
			/*Type *src_type = */sema_expr_resolve_type(sema, expr->as.expr);
			// TODO: check safety
			if (!sema_ast_type(sema, &expr->sema_type, &expr->as.type)) {
				return NULL;
			}
			return &expr->sema_type;
		}
		case EXPR_BINOP: {
			Type *t1 = sema_expr_resolve_type(sema, expr->binop.left);
			Type *t2 = sema_expr_resolve_type(sema, expr->binop.right);
			if (!t1 || !t2) {
				return NULL;
			}
			if (!types_equals(t1, t2)) {
				sema_err("cannot use binop %d for exprs with different types", expr->binop.type);
				return NULL;
			}
			if (!sema_binop_allowed(expr->binop.type, t1)) {
				sema_err("cannot use binop %d for this exprs", expr->binop.type);
				return NULL;
			}
			BinopType comp_binops[] = {
				BINOP_EQ, BINOP_NEQ,
				BINOP_LT, BINOP_LE,
				BINOP_GT, BINOP_GE,
			};
			for (size_t i = 0; i < sizeof(comp_binops)/sizeof(BinopType); i++) {
				if (expr->binop.type == comp_binops[i]) {
					expr->sema_type = primitives[PRIMITIVE_BOOL];
					return &expr->sema_type;
				}
			}
			expr->sema_type = *t1;
			return &expr->sema_type;
		}
/*		case EXPR_UNARY:
			return sema_expr_resolve_type(sema, expr->unary.expr); */
		case EXPR_CHAR:
			return &primitives[PRIMITIVE_U8];
		case EXPR_INTEGER:
			expr->sema_type = primitives[PRIMITIVE_I32];
			return &expr->sema_type;
		case EXPR_BOOL:
			expr->sema_type = primitives[PRIMITIVE_BOOL];
			return &expr->sema_type;
		case EXPR_FUNCALL: {
			Type *type = sema_resolve_funcall_type(sema, &expr->funcall);
			if (type) {
				expr->sema_type = *type;
			}
			return type;
		}
	}
	assert(0 && "unknown expr type");
	return NULL;
}

void sema_module_body(Sema *sema, AstBody *body, Type *returns) {
	Scope scope = scope_new();
	vec_push(&sema->scopes, &scope);
	foreach (&body->stmts, AstStmt, stmt) {
		switch (stmt->type) {
			case AST_STMT_VAR: {
				AstVar *var = &stmt->var;
				Decl decl = { .name = var->name };
				if (var->typed && !sema_ast_type(sema, &decl.type, &var->type)) {
					var->typed = false;
				}
				Type *init_type = var->initializes ? sema_expr_resolve_type(sema, &var->expr) : NULL;
				if (!var->typed && !init_type) {
						sema_err("cannot infer var type(it must be initialized or type must be spicified)");
						break;
				}
				if (var->typed && init_type) {
					if (!types_equals(&decl.type, init_type)) {
						sema_err("specified and initializer types isn't match");
						break;
					}
				} else {
					if (!var->typed) {
						var->typed = true;
						var->type.sema = decl.type = *init_type;
					}
				}
				sema_push_decl(sema, &decl);
				break;
			}
			case AST_STMT_RETURN:
				if (returns) {
					if (!stmt->ret.has_value) {
						if (!types_equals(returns, &primitives[PRIMITIVE_VOID])) {
							sema_err("function must return something but it isn't");
						}
					} else {
						Type *type = sema_expr_resolve_type(sema, &stmt->ret.expr);
						if (!type) {
							break;
						}
						if (!types_equals(returns, type)) {
							sema_err("function specified return type and return expr isn't matches");
						}
					}
				}
				break;
			case AST_STMT_FUNCALL: {
				sema_resolve_funcall_type(sema, &stmt->funcall);
				break;
			}
			case AST_STMT_IF: {
				Type *main = sema_expr_resolve_type(sema, &stmt->if_else.main.expr);
				if (main) {
					if (!types_equals(main, &primitives[PRIMITIVE_BOOL])) {
						sema_err("expected bool as if block condition");
					}
					sema_module_body(sema, &stmt->if_else.main.body, returns);
				}
				foreach (&stmt->if_else.else_ifs, AstIfBody, else_if) {
					Type *else_if_type = sema_expr_resolve_type(sema, &else_if->expr);
					if (else_if_type) {
						if (!types_equals(else_if_type, &primitives[PRIMITIVE_BOOL])) {
							sema_err("expected bool as else if block condition");
						}
					}
					sema_module_body(sema, &else_if->body, returns);
				}
				if (stmt->if_else.has_else) {
					sema_module_body(sema, &stmt->if_else.else_body, returns);
				}
				break;
			}
		}
	}
	vec_pop(&sema->scopes);
}

bool sema_module_node(Sema *sema, AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_FUNC: {
			Scope scope = scope_new();
			vec_push(&sema->scopes, &scope);
			AstFuncDecl *func = &node->func_decl;
			foreach (&func->info.args, AstFuncArg, arg) {
				Decl decl = { .name = arg->name };
				if (sema_ast_type(sema, &decl.type, &arg->type)) {
					sema_push_decl(sema, &decl);
				}
			}
			Type returns;
			sema_module_body(sema, &func->body, sema_ast_type(sema, &returns, &func->info.returning) ? &returns : NULL);
			vec_pop(&sema->scopes);
			return true;
		}
		case AST_MODULE_NODE_EXTERNAL_FUNC: return true;
		default: {
			sema_err("TODO: other module nodes NIY");
			return false;
		}
	}
}

void sema_add_func(Sema *sema, AstFuncInfo *func) {
	Decl decl;
	decl.name = func->name;
	decl.type.type = TYPE_FUNCTION;
	decl.type.func.args = vec_new(TypeFuncArg);
	foreach (&func->args, AstFuncArg, ast_arg) {
		TypeFuncArg arg = {
			.type = malloc(sizeof(Type)),
			.name = ast_arg->name
		};
		sema_ast_type(sema, arg.type, &ast_arg->type);
		vec_push(&decl.type.func.args, &arg);
	}
	decl.type.func.returning = malloc(sizeof(Type));
	sema_ast_type(sema, decl.type.func.returning, &func->returning);
	sema_push_decl(sema, &decl);
}

bool sema_add_module_node(Sema *sema, AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_FUNC: {
			sema_add_func(sema, &node->func_decl.info);
			return true;
		}
		case AST_MODULE_NODE_EXTERNAL_FUNC: {
			sema_add_func(sema, &node->ext_func_decl);
			return true;
		}
	}
	sema_err("invalid module node provided");
	return false;
}

void sema_push_builtin_type(Scope *scope, const char *name, Primitive type) {
	ScopeType result = {
		.name = fatptr_from_cstr(name),
		.type = {
			.type = TYPE_PRIMITIVE,
			.primitive = type
		}
	};
	vec_push(&scope->types, &result);
}

void sema_module(Sema *sema, AstModule *module) {
	Scope scope = scope_new();
	sema_push_builtin_type(&scope, "u8", PRIMITIVE_U8);
	sema_push_builtin_type(&scope, "u16", PRIMITIVE_U16);
	sema_push_builtin_type(&scope, "u32", PRIMITIVE_U32);
	sema_push_builtin_type(&scope, "u64", PRIMITIVE_U64);
	sema_push_builtin_type(&scope, "i8", PRIMITIVE_I8);
	sema_push_builtin_type(&scope, "i16", PRIMITIVE_I16);
	sema_push_builtin_type(&scope, "i32", PRIMITIVE_I32);
	sema_push_builtin_type(&scope, "i64", PRIMITIVE_I64);
	sema_push_builtin_type(&scope, "bool", PRIMITIVE_BOOL);
	sema_push_builtin_type(&scope, "void", PRIMITIVE_VOID);
	vec_push(&sema->scopes, &scope);
	foreach (&module->nodes, AstModuleNode, node) {
		sema_add_module_node(sema, node);
	}
	foreach (&module->nodes, AstModuleNode, node) {
		sema_module_node(sema, node);
	}
}
