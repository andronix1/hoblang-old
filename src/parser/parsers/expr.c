#include "../parsers.h"

void expr_push_up(AstExpr *expr) {
	static int priority[] = {
		[AST_BINOP_ADD] = 10,
		[AST_BINOP_SUB] = 10,
		[AST_BINOP_MUL] = 20,
		[AST_BINOP_DIV] = 20,
		[AST_BINOP_EQ]  = 0,
		[AST_BINOP_NEQ] = 0,
		[AST_BINOP_GT]  = 0,
		[AST_BINOP_GE]  = 0,
		[AST_BINOP_LT]  = 0,
		[AST_BINOP_LE]  = 0,
	};
	if (
		!expr->parent || 
		expr->type != AST_EXPR_BINOP ||
		expr->parent->type != AST_EXPR_BINOP ||
		priority[expr->parent->binop.type] < priority[expr->binop.type]
	) {
		return;
	}
	AstExprBinop *child = &expr->binop;
	AstExprBinop *parent = &expr->parent->binop;
	
	AstBinopType temp_type = child->type;
	child->type = parent->type;
	parent->type = temp_type;

	AstExpr *temp_expr = parent->left;
	parent->left = child->right;
	child->right = child->left;
	child->left = temp_expr;

	expr_push_up(expr->parent);
}

bool expr_make_binop(Parser *parser, AstBinopType type, AstExpr **current_expr, bool(*stop)(TokenType)) {
	AstExpr *nested_expr = malloc(sizeof(AstExpr));				
	memcpy(nested_expr, *current_expr, sizeof(AstExpr));
	(*current_expr)->type = AST_EXPR_BINOP;
	(*current_expr)->binop.type = type;
	(*current_expr)->binop.left = nested_expr;
	(*current_expr)->binop.right = malloc(sizeof(AstExpr));
	nested_expr->parent = *current_expr;
	(*current_expr)->binop.right->parent = *current_expr;
	if (!parse_expr_before(parser, (*current_expr)->binop.right, stop, false)) {
		return false;
	}
	expr_push_up(*current_expr);
	return true;
}

#define AST_EXPR_PARSE_SKIP_NESTED() do { if (stop(token_type(parser->token))) return true; } while (0)
#define PARSE_BINOP(type, priority) do { if (!expr_make_binop(parser, type, &current_expr, stop)) { return false; } AST_EXPR_PARSE_SKIP_NESTED(); } while (0)


bool parse_expr_before(Parser *parser, AstExpr *expr, bool (*stop)(TokenType), bool root) {
	if (root) {
		expr->parent = NULL;
	}
	bool first = false;
	AstExpr *current_expr = expr;
	while (true) {
		parser_next_token(parser);
		if (stop(token_type(parser->token))) {
			if (first) {
				return false;
			}
			return true;
		}
		first = false;
		switch (token_type(parser->token)) {
			case TOKEN_TRUE: case TOKEN_FALSE:
				current_expr->type = AST_EXPR_BOOL;
				current_expr->boolean = parser->token->type == TOKEN_TRUE;
				break;
			case TOKEN_CHAR:
				current_expr->type = AST_EXPR_CHAR;
				current_expr->integer = parser->token->character;
				break;
			case TOKEN_INTEGER: {
				current_expr->type = AST_EXPR_INTEGER;
				current_expr->integer = parser->token->integer;
				break;
			}
			case TOKEN_IDENT: {
				Slice name = parser->token->ident;
				parser_next_token(parser);
				if (token_type(parser->token) != TOKEN_OPENING_CIRCLE_BRACE) {
					current_expr->type = AST_EXPR_VALUE;
					current_expr->value = name;
					parser->skip_next = true;
					break;
				}
				current_expr->type = AST_EXPR_FUNCALL;
				current_expr->func_call.name = name;
				current_expr->func_call.args = vec_new(AstExpr);
				while (token_type(parser->token) != TOKEN_CLOSING_CIRCLE_BRACE) {
					AstExpr arg;
					if (!parse_expr_before(parser, &arg, token_funcall_arg_stop, false)) {
						vec_free(&current_expr->func_call.args);
						return false;
					}
					current_expr->func_call.args = vec_push(current_expr->func_call.args, &arg);
				}
				break;
			}
			case TOKEN_ADD: PARSE_BINOP(AST_BINOP_ADD, 1); break;
			case TOKEN_MINUS: PARSE_BINOP(AST_BINOP_SUB, 1); break;
			case TOKEN_MULTIPLY: PARSE_BINOP(AST_BINOP_MUL, 2); break;
			case TOKEN_DIVIDE: PARSE_BINOP(AST_BINOP_DIV, 2); break;
			case TOKEN_EQUALS: PARSE_BINOP(AST_BINOP_EQ, 0); break;
			case TOKEN_OPENING_CIRCLE_BRACE:
				if (!parse_expr_before(parser, current_expr, token_closing_circle_brace_stop, true)) {
					return false;
				}
				break;
			case TOKEN_EOI:
				parse_err("EOI while parsing expression");
				return false;
			default:
				parse_err("unexpected token `%T` while parsing expression", parser->token);
				return false;
		}
		parser_next_token(parser);	
		if (token_type(parser->token) != TOKEN_AS) {
			parser->skip_next = true;
			continue;
		}
		AstExpr *expr = malloc(sizeof(AstExpr));
		memcpy(expr, current_expr, sizeof(AstExpr));
		current_expr->type = AST_EXPR_AS;
		current_expr->as.expr = expr;
		if (!parse_type(parser, &current_expr->as.type)) {
			return false;
		}
	}
}

bool parse_expr(Parser *parser, AstExpr *expr, bool (*stop)(TokenType)) {
	return parse_expr_before(parser, expr, stop, true);
}
