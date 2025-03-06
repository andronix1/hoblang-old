#include "ast/private/expr.h"
#include "ast/api/expr.h"
#include "ast/private/expr/struct.h"
#include "ast/private/module_node.h"
#include "core/location.h"
#include "lexer/token.h"
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
	pl->binop.right = cr;
	pl->binop.left = pr;

	expr_push_down(pl);
}

bool expr_make_binop(Parser *parser, AstBinopType type, AstExpr **current_expr, bool(*stop)(TokenType)) {
    FileLocation loc = parser_token(parser)->location;
    AstExpr *right = parse_expr(parser, stop);
	if (!right) {
		return false;
	}
    *current_expr = ast_expr_binop(loc, type, *current_expr, right);
	expr_push_down(*current_expr);
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

bool token_struct_stop(TokenType type) {
    return 
        type == TOKEN_CLOSING_FIGURE_BRACE ||
        type == TOKEN_COMMA;
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

        FileLocation loc = token->location;
		
		switch (token->type) {
            case TOKEN_QUESTION_MARK:
                if (!current_expr) {
                    PARSE_ERROR("expected expression before return-on-null operator");
                    return NULL;
                }
                current_expr = ast_expr_ret_on_null(loc, current_expr);
                break;
			case TOKEN_TRUE: case TOKEN_FALSE:
                current_expr = ast_expr_bool(loc, token->type == TOKEN_TRUE);
                break;
			case TOKEN_CHAR: 
                current_expr = ast_expr_char(loc, token->character);
                break;
			case TOKEN_STR:
                current_expr = ast_expr_str(loc, slice_new(token->str, vec_len(token->str)));
                break;
			case TOKEN_OPENING_SQUARE_BRACE:
				if (!current_expr) {
					PARSE_ERROR("expected expression before index expression");
				}
                current_expr = ast_expr_idx(loc, current_expr, parse_expr(parser, token_stop_idx));
				if (!current_expr) {
					return NULL;
				}
				PARSER_EXPECT_NEXT(TOKEN_CLOSING_SQUARE_BRACE, "index closing brace");
				break;
			case TOKEN_INTEGER:
                current_expr = ast_expr_integer(loc, token->integer);
                break;
			case TOKEN_FLOAT:
                current_expr = ast_expr_float(loc, token->float_value);
                break;
			case TOKEN_STRUCT: {
                AstPath path;
                if (!parse_path(parser, &path)) {
                    return NULL;
                }
                PARSER_EXPECT_NEXT(TOKEN_OPENING_FIGURE_BRACE, "opening figure brace");
                AstExprStructMember *members = vec_new(AstExprStructMember);
                while (true) {
                    AstExprStructMember member;
                    member.name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "field name")->ident;
                    PARSER_EXPECT_NEXT(TOKEN_COLON, "colon");
                    if (!(member.expr = parse_expr(parser, token_struct_stop))) {
                        return NULL;
                    }
                    members = vec_push(members, &member);
                    if (parser_next(parser)->type == TOKEN_CLOSING_FIGURE_BRACE) {
                        break;
                    }
                }
                return ast_expr_struct(loc, path, members);
            }
			case TOKEN_IDENT: {
				parser_skip_next(parser);
				AstPath path;
				if (!parse_path(parser, &path)) {
					return NULL;
				}
				current_expr= ast_expr_get_local_path(loc, path);
				break;
			}
            case TOKEN_UNWRAP:
                if (!current_expr) {
					PARSE_ERROR("expected expression before index expression");
                    return NULL;
                }
                current_expr = ast_expr_unwrap(loc, current_expr, PARSER_EXPECT_NEXT(TOKEN_IDENT, "output name")->ident);
                break;
            case TOKEN_NULL:
                current_expr = ast_expr_null(loc);
                break;
			case TOKEN_NOT: 
                current_expr = ast_expr_not(loc, NOT_NULL(parse_expr(parser, stop)));
                break;
			case TOKEN_BITAND: {
				if (first) {
					current_expr = ast_expr_ref(loc, NOT_NULL(_parse_expr(parser, stop, false)));
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
                if (current_expr) {
					current_expr = ast_expr_call(loc, current_expr, NOT_NULL(parse_call_args(parser)));
                } else {
                    current_expr = parse_expr(parser, token_stop_closing_circle_brace);
                    current_expr->scoped = true;
                    current_expr->loc = loc;
                    PARSER_EXPECT_NEXT(TOKEN_CLOSING_CIRCLE_BRACE, "scope close");
                }
                break;
			case TOKEN_OPENING_FIGURE_BRACE: {
				AstExpr **values = vec_new(AstExpr*);
				while (token->type != TOKEN_CLOSING_FIGURE_BRACE) {
					AstExpr *expr = parse_expr(parser, token_stop_array_arg);
					if (!expr) {
						return NULL;
					}
					values = vec_push(values, &expr);
					token = parser_next(parser);
				}
				current_expr = ast_expr_array(loc, values);
				break;
            }
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
                    FileLocation as_loc = token->location;
                    if (parser_next_is_not(parser, TOKEN_AUTO)) {
                        AstType type;
                        if (!parse_type(parser, &type)) {
                            return NULL;
                        }
                        current_expr = ast_expr_as_type(loc, as_loc, current_expr, type);
                    } else {
                        current_expr = ast_expr_as_auto(loc, as_loc, current_expr);
                    }
                    break;
                }
				case TOKEN_DOT: {
					AstPath path;
					if (!parse_path(parser, &path)) {
						return NULL;
					}
					current_expr = ast_expr_get_inner_path(loc, current_expr, path);
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
