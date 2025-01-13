#include "../parsers.h"

void expr_push_down(AstExpr *expr) {
	static int priority[] = {
		[AST_BINOP_MUL] = 20,
		[AST_BINOP_DIV] = 20,
		[AST_BINOP_ADD] = 10,
		[AST_BINOP_SUB] = 10,
		[AST_BINOP_EQ]  = 0,
		[AST_BINOP_NEQ] = 0,
		[AST_BINOP_GT]  = 0,
		[AST_BINOP_GE]  = 0,
		[AST_BINOP_LT]  = 0,
		[AST_BINOP_LE]  = 0,
	};
	
	AstExpr *pr = expr->binop.left,
			*pl = expr->binop.right;

	if (
		expr->type != AST_EXPR_BINOP ||
		pl->type != AST_EXPR_BINOP ||
		priority[pl->binop.type] > priority[expr->binop.type]
	) return;

	AstExpr *cr = pl->binop.left,
			*cl = pl->binop.right;

	AstBinopType pb = expr->binop.type,
				 cb = pl->binop.type;

	expr->binop.type = cb;
	expr->binop.right = cl;
	expr->binop.left = pl;
	pl->binop.type = pb;
	pl->binop.right= cr;
	pl->binop.left = pr;

	expr_push_down(pl);
}

bool expr_make_binop(Parser *parser, AstBinopType type, AstExpr **current_expr, bool(*stop)(TokenType)) {
	AstExpr *result = malloc(sizeof(AstExpr));				
	result->type = AST_EXPR_BINOP;
	result->binop.type = type;
	result->binop.left = *current_expr;
	if (!(result->binop.right = parse_expr_before(parser, stop))) {
		return false;
	}
	expr_push_down(result);
	*current_expr = result;
	return true;
}

#define AST_EXPR_PARSE_SKIP_NESTED() do { if (stop(token_type(parser->token))) return current_expr; } while (0)
#define PARSE_BINOP(type) do { if (!expr_make_binop(parser, type, &current_expr, stop)) { return NULL; } AST_EXPR_PARSE_SKIP_NESTED(); } while (0)


AstExpr *parse_expr_before(Parser *parser, bool (*stop)(TokenType)) {
	bool first = false;
	AstExpr *current_expr = malloc(sizeof(AstExpr));
	while (true) {
		parser_next_token(parser);
		if (stop(token_type(parser->token))) {
			if (first) {
				return NULL;
			}
			return current_expr;
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
			case TOKEN_STR: {
				current_expr->type = AST_EXPR_STR;
				current_expr->str.len = vec_len(parser->token->str);
				current_expr->str.str= parser->token->str;
				break;
			}
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
				if (!parse_func_call_args(parser, &current_expr->func_call)) {
					return false;
				}
				break;
			}
			case TOKEN_ADD: PARSE_BINOP(AST_BINOP_ADD); break;
			case TOKEN_MINUS: PARSE_BINOP(AST_BINOP_SUB); break;
			case TOKEN_MULTIPLY: PARSE_BINOP(AST_BINOP_MUL); break;
			case TOKEN_DIVIDE: PARSE_BINOP(AST_BINOP_DIV); break;
			case TOKEN_EQUALS: PARSE_BINOP(AST_BINOP_EQ); break;
			case TOKEN_NOT_EQUALS: PARSE_BINOP(AST_BINOP_NEQ); break;
			case TOKEN_LESS: PARSE_BINOP(AST_BINOP_LT); break;
			case TOKEN_LESS_OR_EQUALS: PARSE_BINOP(AST_BINOP_LE); break;
			case TOKEN_GREATER: PARSE_BINOP(AST_BINOP_GT); break;
			case TOKEN_GREATER_OR_EQUALS: PARSE_BINOP(AST_BINOP_GE); break;
			case TOKEN_OPENING_CIRCLE_BRACE:
				if (!(current_expr = parse_expr_before(parser, token_closing_circle_brace_stop))) {
					return NULL;
				}
				break;
			case TOKEN_OPENING_SQUARE_BRACE:
				current_expr->type = AST_EXPR_ARRAY;
				current_expr->array = vec_new(AstExpr);
				while (token_type(parser->token) != TOKEN_CLOSING_SQUARE_BRACE) {
					AstExpr *expr = parse_expr_before(parser, token_array_arg_stop);
					if (!expr) {
						return false;
					}
					current_expr->array = vec_push(current_expr->array, expr);
				}
				break;
			case TOKEN_EOI:
				parse_err("EOI while parsing expression");
				return NULL;
			default:
				parse_err("unexpected token `{tok}` while parsing expression", parser->token);
				return NULL;
		}
		parser_next_token(parser);	
		switch (token_type(parser->token)) {
			case TOKEN_AS: {
				AstExpr *expr = malloc(sizeof(AstExpr));
				expr->type = AST_EXPR_AS;
				expr->as.expr = current_expr;
				current_expr = expr;
				if (!parse_type(parser, &current_expr->as.type)) {
					return NULL;
				}
				break;
			}
			case TOKEN_OPENING_SQUARE_BRACE: {
				AstExpr *expr = malloc(sizeof(AstExpr));
				expr->type = AST_EXPR_IDX;
				expr->idx.expr = current_expr;
				if (!(expr->idx.idx = parse_expr_before(parser, token_slice_at_stop))) {
					return NULL;
				}
				current_expr = expr;
				break;
			}
			default:
				parser->skip_next = true;
				break;
		}
	}
}

bool parse_expr(Parser *parser, AstExpr *expr, bool (*stop)(TokenType)) {
	AstExpr *result = parse_expr_before(parser, stop);
	if (!result) {
		return false;
	}
	memcpy(expr, result, sizeof(AstExpr));
	free(result);
	return true;
}
