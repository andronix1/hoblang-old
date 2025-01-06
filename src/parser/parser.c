#include "parser.h"

#define EXPECTED(what) "expected " what " found `%T`", parser->token

#define parse_exp(type, what) \
	do { \
		if (token_type(parser->token) != type) { \
			parse_err(EXPECTED(what)); \
			return false; \
		} \
	} while (0)

#define parse_exp_next(type, what) \
	do { \
		parser_next_token(parser); \
		parse_exp(type, what); \
	} while (0)

void parser_next_token(Parser *parser) {
	if (parser->skip_next) {
		parser->skip_next = false;
		return;
	}
	if (!lex_next(parser->lexer)) {
		parser->token = NULL;
	} else {
		parser->token = &parser->lexer->token;
	}
}

bool parse_type(Parser *parser, AstType *type) {
	parser_next_token(parser);
	switch (token_type(parser->token)) {
		case TOKEN_IDENT:
			type->type = AST_TYPE_IDENT;
			type->ident = parser->token->ident;
			return true;
		default:
			parse_err(EXPECTED("type"));
			return false;
	}
}

bool token_semicolon_stop(TokenType type) { return type == TOKEN_SEMICOLON; }
bool token_closing_circle_brace_stop(TokenType type) { return type == TOKEN_CLOSING_CIRCLE_BRACE; }
bool token_opening_figure_brace_stop(TokenType type) { return type == TOKEN_OPENING_FIGURE_BRACE; }
bool token_funcall_arg_stop(TokenType type) { return type == TOKEN_COMMA || type == TOKEN_CLOSING_CIRCLE_BRACE; }

bool parse_expr_before(Parser *parser, Expr *expr, bool (*stop)(TokenType), bool root);

void expr_push_up(Expr *expr) {
	assert(expr->type == EXPR_BINOP);
	static int priority[] = {
		[BINOP_ADD] = 10,
		[BINOP_SUB] = 10,
		[BINOP_MUL] = 20,
		[BINOP_DIV] = 20,
		[BINOP_EQ]  = 0,
		[BINOP_NEQ] = 0,
		[BINOP_GT]  = 0,
		[BINOP_GE]  = 0,
		[BINOP_LT]  = 0,
		[BINOP_LE]  = 0,
	};
	if (
		!expr->parent || 
		expr->type == EXPR_AS ||
		expr->parent->type != EXPR_BINOP ||
		priority[expr->parent->binop.type] < priority[expr->binop.type]
	) {
		return;
	}
	ExprBinop *child = &expr->binop;
	ExprBinop *parent = &expr->parent->binop;
	
	BinopType temp_type = child->type;
	child->type = parent->type;
	parent->type = temp_type;

	Expr *temp_expr = parent->left;
	parent->left = child->right;
	child->right = child->left;
	child->left = temp_expr;

	expr_push_up(expr->parent);
}

bool expr_make_binop(Parser *parser, BinopType type, Expr **current_expr, bool(*stop)(TokenType)) {
	Expr *nested_expr = malloc(sizeof(Expr));				
	memcpy(nested_expr, *current_expr, sizeof(Expr));
	(*current_expr)->type = EXPR_BINOP;
	(*current_expr)->binop.type = type;
	(*current_expr)->binop.left = nested_expr;
	(*current_expr)->binop.right = malloc(sizeof(Expr));
	nested_expr->parent = *current_expr;
	(*current_expr)->binop.right->parent = *current_expr;
	if (!parse_expr_before(parser, (*current_expr)->binop.right, stop, false)) {
		return false;
	}
	expr_push_up(*current_expr);
	return true;
}

#define EXPR_PARSE_SKIP_NESTED() do { if (stop(token_type(parser->token))) return true; } while (0)
#define PARSE_BINOP(type, priority) do { if (!expr_make_binop(parser, type, &current_expr, stop)) { return false; } EXPR_PARSE_SKIP_NESTED(); } while (0)
#define EXPR_PARSE_FREE() do { if (root) expr_free(expr); } while (0)

void parse_skip_to(Parser *parser, TokenType stop) {
	while (token_type(parser->token) != stop && token_type(parser->token) != TOKEN_EOI) {
		parser_next_token(parser);
	}
}

bool parse_expr_before(Parser *parser, Expr *expr, bool (*stop)(TokenType), bool root) {
	if (root) {
		expr->parent = NULL;
	}
	bool first = false;
	Expr *current_expr = expr;
	while (true) {
		parser_next_token(parser);
		if (stop(token_type(parser->token))) {
			if (first) {
				EXPR_PARSE_FREE();
				return false;
			}
			return true;
		}
		first = false;
		switch (token_type(parser->token)) {
			case TOKEN_TRUE: case TOKEN_FALSE:
				current_expr->type = EXPR_BOOL;
				current_expr->boolean = parser->token->type == TOKEN_TRUE;
				break;
			case TOKEN_CHAR:
				current_expr->type = EXPR_CHAR;
				current_expr->integer = parser->token->character;
				break;
			case TOKEN_INTEGER: {
				current_expr->type = EXPR_INTEGER;
				current_expr->integer = parser->token->integer;
				break;
			}
			case TOKEN_IDENT: {
				FatPtr name = parser->token->ident;
				parser_next_token(parser);
				if (token_type(parser->token) != TOKEN_OPENING_CIRCLE_BRACE) {
					current_expr->type = EXPR_IDENT;
					current_expr->ident = name;
					parser->skip_next = true;
					break;
				}
				current_expr->type = EXPR_FUNCALL;
				current_expr->funcall.name = name;
				current_expr->funcall.args = vec_new(Expr);
				while (token_type(parser->token) != TOKEN_CLOSING_CIRCLE_BRACE) {
					Expr arg;
					if (!parse_expr_before(parser, &arg, token_funcall_arg_stop, false)) {
						vec_free(&current_expr->funcall.args);
						return false;
					}
					vec_push(&current_expr->funcall.args, &arg);
				}
				break;
			}
			case TOKEN_ADD: PARSE_BINOP(BINOP_ADD, 1); break;
			case TOKEN_MINUS: PARSE_BINOP(BINOP_SUB, 1); break;
			case TOKEN_MULTIPLY: PARSE_BINOP(BINOP_MUL, 2); break;
			case TOKEN_DIVIDE: PARSE_BINOP(BINOP_DIV, 2); break;
			case TOKEN_EQUALS: PARSE_BINOP(BINOP_EQ, 0); break;
			case TOKEN_OPENING_CIRCLE_BRACE:
				if (!parse_expr_before(parser, current_expr, token_closing_circle_brace_stop, true)) {
					EXPR_PARSE_FREE();
					return false;
				}
				break;
			case TOKEN_EOI:
				parse_err("EOI while parsing expression");
				EXPR_PARSE_FREE();
				return false;
			default:
				parse_err("unexpected token `%T` while parsing expression", parser->token);
				EXPR_PARSE_FREE();
				return false;
		}
		parser_next_token(parser);	
		if (token_type(parser->token) != TOKEN_AS) {
			parser->skip_next = true;
			continue;
		}
		Expr *expr = malloc(sizeof(Expr));
		memcpy(expr, current_expr, sizeof(Expr));
		current_expr->type = EXPR_AS;
		current_expr->as.expr = expr;
		if (!parse_type(parser, &current_expr->as.type)) {
			return false;
		}
	}
}

bool parse_return(Parser *parser, AstReturn *ret) {
	parser_next_token(parser);
	if (token_type(parser->token) == TOKEN_SEMICOLON) {
		ret->has_value = false;
		return true;
	}
	ret->has_value = true;
	parser->skip_next = true;
	return parse_expr_before(parser, &ret->expr, token_semicolon_stop, true);
}

bool parse_var(Parser *parser, AstVar *var) {
	parse_exp_next(TOKEN_IDENT, "variable name");
	var->name = parser->token->ident;
	parser_next_token(parser);
	var->typed = false;
	switch (token_type(parser->token)) {
		case TOKEN_SEMICOLON:
			return true;
		case TOKEN_ASSIGN:
			var->initializes = true;
			return parse_expr_before(parser, &var->expr, token_semicolon_stop, true);
		case TOKEN_COLON:
			var->typed = true;
			if (!parse_type(parser, &var->type)) {
				return false;
			}
			break;
		default:
			parse_err("expected type or assign but got `%T`", parser->token);
			return false;
	}
	parser_next_token(parser);
	switch (token_type(parser->token)) {
		case TOKEN_ASSIGN:
			var->initializes = true;
			return parse_expr_before(parser, &var->expr, token_semicolon_stop, true);
		case TOKEN_SEMICOLON:
			var->initializes = false;
			return true;
		default:
			parse_err("expected initilizer or end but got `%T`", parser->token);
			return false;
	}
}

bool parse_funcall_args(Parser *parser, Funcall *funcall) {
	parser_next_token(parser);
	funcall->args = vec_new(Expr);
	bool first = true;
	while (token_type(parser->token) != TOKEN_CLOSING_CIRCLE_BRACE) {
		Expr expr;
		parser->skip_next = first;
		first = false;
		if (!parse_expr_before(parser, &expr, token_funcall_arg_stop, true)) {
			return false;
		}
		vec_push(&funcall->args, &expr);
	}
	return true;
}

bool parse_body(Parser *parser, AstBody *body);

bool parse_if_else_body(Parser *parser, AstIfBody *body) {
	if (!parse_expr_before(parser, &body->expr, token_opening_figure_brace_stop, true)) {
		return false;
	}
	parser->skip_next = true;
	return parse_body(parser, &body->body);
}

bool parse_if_else(Parser *parser, AstIf *if_else) {
	if_else->else_ifs = vec_new(AstIfBody);
	if_else->has_else = false;
	if (!parse_if_else_body(parser, &if_else->main)) {
		return false;
	}
	parser_next_token(parser);
	while (token_type(parser->token) == TOKEN_ELSE) {
		parser_next_token(parser);
		switch (token_type(parser->token)) {
			case TOKEN_IF: {
				AstIfBody body;
				if (!parse_if_else_body(parser, &body)) {
					return false;
				}
				vec_push(&if_else->else_ifs, &body);
				break;
			}
			case TOKEN_OPENING_FIGURE_BRACE:
				parser->skip_next = true;
				if (!parse_body(parser, &if_else->else_body)) {
					return false;
				}
				if_else->has_else = true;
				return true;
			default:
				parse_err("expected else if or else body but got `%T`", parser->token);
				return false;
		}
		parser_next_token(parser);
	}
	parser->skip_next = true;
	return true;
}

bool parse_stmt(Parser *parser, AstStmt *stmt) {
	parser_next_token(parser);
	switch (token_type(parser->token)) {
		case TOKEN_IDENT: {
			FatPtr name = parser->token->ident;
			parser_next_token(parser);
			switch (token_type(parser->token)) {
				case TOKEN_OPENING_CIRCLE_BRACE:
					stmt->type = AST_STMT_FUNCALL;
					stmt->funcall.name = name;
					return parse_funcall_args(parser, &stmt->funcall);
				default:
					parse_err("unexpected token `%T` after ident in statement");
					return false;
			}
		}
		case TOKEN_IF:
			stmt->type = AST_STMT_IF;
			return parse_if_else(parser, &stmt->if_else);
		case TOKEN_RETURN:
			stmt->type = AST_STMT_RETURN;
			return parse_return(parser, &stmt->ret);
		case TOKEN_VAR:
			stmt->type = AST_STMT_VAR;
			return parse_var(parser, &stmt->var);
		default:
			parse_err("unexpected `%T` while parsing statement", parser->token);
			return false;
	}
}

bool parse_body(Parser *parser, AstBody *body) {
	body->stmts = vec_new(AstStmt);
	parse_exp_next(TOKEN_OPENING_FIGURE_BRACE, "body opening");
	while (true) {
		parser_next_token(parser);
		switch (token_type(parser->token)) {
			case TOKEN_SEMICOLON:
				break;
			case TOKEN_CLOSING_FIGURE_BRACE:
				return true;
			case TOKEN_EOI:
				parse_err("EOI while parsing body");
				return false;
			default: {
				parser->skip_next = true;
				AstStmt stmt;
				if (parse_stmt(parser, &stmt)) {
					vec_push(&body->stmts, &stmt);
				}
				break;
			}
		}
	}
	parse_exp_next(TOKEN_CLOSING_FIGURE_BRACE, "body closing");
	return false;
}

bool parse_func_info(Parser *parser, AstFuncInfo *info) {
	parse_exp_next(TOKEN_IDENT, "function name");
	info->name = parser->token->ident;

	parse_exp_next(TOKEN_OPENING_CIRCLE_BRACE, "opening args brace");
	bool parsing_args = true;
	bool was_arg = false;
	info->args = vec_new(AstFuncArg);
	while (parsing_args) {
		parser_next_token(parser);
		switch (token_type(parser->token)) {
			case TOKEN_CLOSING_CIRCLE_BRACE:
				parsing_args = false;
				break;
			case TOKEN_COMMA:
				if (!was_arg) {
					parse_err("commas only after args allowed");
					return false;
				}
				was_arg = false;
				break;
			case TOKEN_IDENT: {
				AstFuncArg arg;
				arg.name = parser->token->ident;
				parse_exp_next(TOKEN_COLON, "arg type delimeter");
				if (!parse_type(parser, &arg.type)) {
					return false;
				}
				vec_push(&info->args, &arg);
				was_arg = true;
				break;
			}
			default:
				parse_err(EXPECTED("function args"));
				return false;
		}
	}

	parse_exp_next(TOKEN_COLON, "returning type");
	parse_type(parser, &info->returning);
	return true;
}

bool parse_external(Parser *parser, AstFuncInfo *info) {
	parse_exp_next(TOKEN_FUN, "function");
	if (!parse_func_info(parser, info)) {
		return false;
	}
	return true;
}

bool parse_func_decl(Parser *parser, AstFuncDecl *decl) {
	if (!parse_func_info(parser, &decl->info)) {
		return false;
	}
	parse_body(parser, &decl->body);
	return true;
}

bool parse_module_node(Parser *parser, AstModuleNode *node) {
	parser_next_token(parser);
	switch (token_type(parser->token)) {
		case TOKEN_FUN:
			node->type = AST_MODULE_NODE_FUNC;
			return parse_func_decl(parser, &node->func_decl);
		case TOKEN_EXTERN:
			node->type = AST_MODULE_NODE_EXTERNAL_FUNC;
			return parse_external(parser, &node->ext_func_decl);
		default:
			parse_err("unexpected `%T` while parsing module node", parser->token);
			return false;
	}
}

bool parse_module(Parser *parser, AstModule *module) {
	module->nodes = vec_new(AstModuleNode);
	while (true) {
		AstModuleNode node;
		parser_next_token(parser);
		if (token_type(parser->token) == TOKEN_EOI) {
			return true;
		}
		parser->skip_next = true;
		if (!parse_module_node(parser, &node)) {
			return false;
		}
		vec_push(&module->nodes, &node);
	}
	return true;
}
