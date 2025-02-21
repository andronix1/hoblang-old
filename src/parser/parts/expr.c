#include "ast/private/expr.h"
#include "parser/parts/expr.h"
#include "parser/parts/type.h"
#include "parser/parts/path.h"
#include "parser/private.h"
#include "parser/token_stops.h"

void expr_push_down(AstExpr *expr) {
	static int priority[] = {
		[AST_BINOP_BITAND] = 30,
		[AST_BINOP_BITOR] =  15,
		[AST_BINOP_SHR] =    30,
		[AST_BINOP_SHL] =    30,
		[AST_BINOP_XOR] =    30,
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
		[AST_BINOP_OR]  = -1,
		[AST_BINOP_AND] = -1,
	};
	
	AstExpr *pr = expr->binop.left,
			*pl = expr->binop.right;

	if (pl->scoped) {
		return;
	}

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
	if (!(result->binop.right = parse_expr(parser, stop))) {
		return false;
	}
	expr_push_down(result);
	*current_expr = result;
	return true;
}

AstExpr *expr_make_unary(Parser *parser, AstUnaryType type, bool(*stop)(TokenType)) {
	AstExpr *result = malloc(sizeof(AstExpr));				
	result->type = AST_EXPR_UNARY;
	result->unary.type = type;
	if (!(result->unary.expr = parse_expr(parser, stop))) {
		return NULL;
	}
	return result;
}

#define AST_EXPR_PARSE_SKIP_NESTED() do { if (stop(parser_token(parser)->type)) return current_expr; } while (0)
#define PARSE_BINOP(type) do { \
		if (first) { \
			PARSE_ERROR("binary operator without left hand expression"); \
			return NULL; \
		} \
		if (!expr_make_binop(parser, type, &current_expr, stop)) { \
			return NULL; \
		} \
		AST_EXPR_PARSE_SKIP_NESTED(); \
	} while (0)
#define PARSE_BINOP_MAYBE_UNARY(unary_type, binop_type) do { \
		if (first) { \
			return expr_make_unary(parser, unary_type, stop); \
		} else { \
			PARSE_BINOP(binop_type); \
		} \
	} while (0)
#define NOT_NULL(expr) ({typeof(expr) _expr = expr; if (!_expr) { return NULL; } _expr; })

AstExpr **parse_call_args(Parser *parser) {
	AstExpr** args = vec_new(AstExpr*);
	if (parser_next_is(parser, TOKEN_CLOSING_CIRCLE_BRACE)) {
		return args;
	}
	while (true) {
		AstExpr *expr = parse_expr(parser, token_stop_funcall_arg);
		if (!expr) {
			return false;
		}
		args = vec_push(args, &expr);
		switch (parser_next(parser)->type) {
			case TOKEN_COMMA: break;
			case TOKEN_CLOSING_CIRCLE_BRACE: return args;
			default:
				PARSE_ERROR(EXPECTED("arg stop"));
				return NULL;
		}
	}
}

AstExpr *_parse_expr(Parser *parser, bool (*stop)(TokenType), bool post_parse) {
	bool first = true;
	AstExpr *current_expr = NULL;
	while (true) {
		Token *token = parser_next(parser);
		if (stop(token->type)) {
            parser_skip_next(parser);
			if (!current_expr) {
                PARSE_ERROR("unexpected end of expression");
			}
			return current_expr;
		}
		
		switch (token->type) {
			case TOKEN_TRUE: case TOKEN_FALSE:
                current_expr = ast_expr_bool(token->type == TOKEN_TRUE);
                break;
			case TOKEN_CHAR: 
                current_expr = ast_expr_char(token->character);
                break;
			case TOKEN_STR:
                current_expr = ast_expr_str(slice_new(token->str, vec_len(token->str)));
                break;
			case TOKEN_OPENING_SQUARE_BRACE:
				if (!current_expr) {
					PARSE_ERROR("expected expression before index expression");
				}
                current_expr = ast_expr_idx(current_expr, parse_expr(parser, token_stop_idx));
				if (!current_expr) {
					return NULL;
				}
				PARSER_EXPECT_NEXT(TOKEN_CLOSING_SQUARE_BRACE, "index closing brace");
				break;
			case TOKEN_INTEGER:
                current_expr = ast_expr_integer(token->integer);
                break;
			case TOKEN_FLOAT:
                current_expr = ast_expr_float(token->float_value);
                break;
			case TOKEN_IDENT: {
				parser_skip_next(parser);
				AstPath path;
				if (!parse_path(parser, &path)) {
					return NULL;
				}
				AstExpr *expr = ast_expr_get_local_path(path);
				while (parser_next_is(parser, TOKEN_OPENING_CIRCLE_BRACE)) {
					expr = ast_expr_call(expr, NOT_NULL(parse_call_args(parser)));
				}
				current_expr = expr;
				break;
			}
            case TOKEN_NULL:
                current_expr = ast_expr_null();
                break;
			case TOKEN_NOT: 
                current_expr = ast_expr_not(NOT_NULL(parse_expr(parser, stop)));
                break;
			case TOKEN_BITAND: {
				if (first) {
					current_expr = ast_expr_ref(NOT_NULL(_parse_expr(parser, stop, false)));
				} else {
					PARSE_BINOP(AST_BINOP_BITAND);
				}
				break;
			}
			case TOKEN_MOD: PARSE_BINOP(AST_BINOP_MOD); break;
			case TOKEN_ADD: PARSE_BINOP(AST_BINOP_ADD); break;
			case TOKEN_AND: PARSE_BINOP(AST_BINOP_AND); break;
			case TOKEN_XOR: PARSE_BINOP(AST_BINOP_XOR); break;
			case TOKEN_BITOR: PARSE_BINOP(AST_BINOP_BITOR); break;
			case TOKEN_BITNOT: 
                current_expr = expr_make_unary(parser, AST_UNARY_BITNOT, stop);
                break;
			case TOKEN_SHR: PARSE_BINOP(AST_BINOP_SHR); break;
			case TOKEN_SHL: PARSE_BINOP(AST_BINOP_SHL); break;
			case TOKEN_MINUS: PARSE_BINOP_MAYBE_UNARY(AST_UNARY_MINUS, AST_BINOP_SUB); break;
			case TOKEN_MULTIPLY: PARSE_BINOP(AST_BINOP_MUL); break;
			case TOKEN_DIVIDE: PARSE_BINOP(AST_BINOP_DIV); break;
			case TOKEN_EQUALS: PARSE_BINOP(AST_BINOP_EQ); break;
			case TOKEN_NOT_EQUALS: PARSE_BINOP(AST_BINOP_NEQ); break;
			case TOKEN_LESS: PARSE_BINOP(AST_BINOP_LT); break;
			case TOKEN_LESS_OR_EQUALS: PARSE_BINOP(AST_BINOP_LE); break;
			case TOKEN_GREATER: PARSE_BINOP(AST_BINOP_GT); break;
			case TOKEN_GREATER_OR_EQUALS: PARSE_BINOP(AST_BINOP_GE); break;
			case TOKEN_OR: PARSE_BINOP(AST_BINOP_OR); break;
			case TOKEN_OPENING_CIRCLE_BRACE:
                current_expr = parse_expr(parser, token_stop_closing_circle_brace);
				current_expr->scoped = true;
				PARSER_EXPECT_NEXT(TOKEN_CLOSING_CIRCLE_BRACE, "scope close");
                break;
			case TOKEN_OPENING_FIGURE_BRACE:
				AstExpr **values = vec_new(AstExpr*);
				while (token->type != TOKEN_CLOSING_FIGURE_BRACE) {
					AstExpr *expr = parse_expr(parser, token_stop_array_arg);
					if (!expr) {
						return NULL;
					}
					values = vec_push(values, &expr);
					token = parser_next(parser);
				}
				current_expr = ast_expr_array(values);
				break;
			case TOKEN_EOI:
				PARSE_ERROR("EOI while parsing expression");
				return NULL;
			default:
				PARSE_ERROR("unexpected token `{tok}` while parsing expression", parser_token(parser));
				return NULL;
		}
        bool reading = true;
        while (reading) {
            Token *token = parser_next(parser);	
            if (stop(token->type)) {
                parser_skip_next(parser);
                break;
            }
            switch (token->type) {
                case TOKEN_AS: {
					if (!post_parse) {
						parser_skip_next(parser);
						return current_expr;
					}
                    AstExpr *expr = malloc(sizeof(AstExpr));
                    expr->type = AST_EXPR_AS;
                    expr->as.expr = current_expr;
                    current_expr = expr;
                    if (!parse_type(parser, &current_expr->as.type)) {
                        return NULL;
                    }
                    break;
                }
				case TOKEN_DOT: {
					AstInnerPath path;
					if (!parse_inner_path(parser, &path)) {
						return NULL;
					}
					current_expr = ast_expr_get_inner_path(current_expr, path);
					break;
				}
                default:
                    parser_skip_next(parser);
                    reading = false;
                    break;
            }
            first = false;
        }
	}
}

AstExpr *parse_expr(Parser *parser, bool (*stop)(TokenType)) {
	return _parse_expr(parser, stop, true);
}
