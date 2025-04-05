#include "expr.h"
#include "core/location.h"
#include "core/not_null.h"
#include "core/vec.h"
#include "parser/api.h"
#include "ast/shared/expr.h"
#include "parser/ast/shared/body.h"
#include "parser/ast/shared/path.h"
#include "parser/api.h"
#include "parser/ast/shared/type.h"

AstExpr *parse_expr_middle(Parser *parser) {
    Token *token = parser_next(parser);
    FileLocation loc = token->location;
    switch (token->kind) {
        case TOKEN_IDENT:
            parser_skip_next(parser);
            return ast_expr_new_local_path(loc, NOT_NULL(parse_path(parser)));
        case TOKEN_INTEGER: return ast_expr_new_integer(loc, token->integer);
        case TOKEN_CHAR: return ast_expr_new_character(loc, token->character);
        case TOKEN_FLOAT: return ast_expr_new_fp(loc, token->float_value);
        case TOKEN_STR: return ast_expr_new_str(loc, token->str);
        case TOKEN_TRUE: return ast_expr_new_bool(loc, true);
        case TOKEN_FALSE: return ast_expr_new_bool(loc, false);
        case TOKEN_NULL: return ast_expr_new_null(loc );
        case TOKEN_OPENING_CIRCLE_BRACE: {
            AstExpr *expr = NOT_NULL(parse_expr(parser));
            PARSER_EXPECT_NEXT(TOKEN_CLOSING_CIRCLE_BRACE, ")");
            return ast_expr_new_scope(loc, expr);
        }
        case TOKEN_STRUCT: {
            AstPath *path = parse_path(parser);
            AstExprStructureField *fields = vec_new(AstExprStructureField);
            PARSER_EXPECT_NEXT(TOKEN_OPENING_FIGURE_BRACE, "{");
            while (parser_next_is_not(parser, TOKEN_CLOSING_FIGURE_BRACE)) {
                Token *name_token = PARSER_EXPECT_NEXT(TOKEN_IDENT, "field name");
                Slice name = name_token->ident;
                FileLocation name_loc = name_token->location;
                PARSER_EXPECT_NEXT(TOKEN_COLON, "colon");
                AstExpr *value = NOT_NULL(parse_expr(parser));
                fields = vec_push_dir(fields, ast_expr_structure_field(name, name_loc, value));
                switch (parser_next(parser)->kind) {
                    case TOKEN_COMMA: break;
                    case TOKEN_CLOSING_FIGURE_BRACE:
                        parser_skip_next(parser);
                        break;
                    default:
                        PARSE_ERROR(EXPECTED("comma or }"));
                        return NULL;
                }
            }
            return ast_expr_new_structure(loc, path, fields);
        }
        case TOKEN_FUN: {
            PARSER_EXPECT_NEXT(TOKEN_OPENING_CIRCLE_BRACE, "(");
            AstExprAnonFuncArg *args = vec_new(AstExprAnonFuncArg);
            while (parser_next_is_not(parser, TOKEN_CLOSING_CIRCLE_BRACE)) {
                Slice name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "arg name")->ident;
                AstType *type = NULL;
                if (parser_next_is(parser, TOKEN_COLON)) {
                    type = NOT_NULL(parse_type(parser));
                }
                args = vec_push_dir(args, ast_expr_anon_fun_arg_new(name, type));
                switch (parser_next(parser)->kind) {
                    case TOKEN_COMMA: break;
                    case TOKEN_CLOSING_CIRCLE_BRACE:
                        parser_skip_next(parser);
                        break;
                    default:
                        PARSE_ERROR(EXPECTED("comma or }"));
                        return NULL;
                }
            }
            AstType *returns = NULL;
            if (parser_next_is(parser, TOKEN_FUNC_RETURNS)) {
                returns = NOT_NULL(parse_type(parser)); 
            }
            AstBody *body = NOT_NULL(parse_body(parser));
            return ast_expr_new_anon_func(loc, args, returns, body);
        }
        case TOKEN_OPENING_FIGURE_BRACE: {
            AstExpr **values = vec_new(AstExpr*);
            while (parser_next_is_not(parser, TOKEN_CLOSING_FIGURE_BRACE)) {
                values = vec_push_dir(values, NOT_NULL(parse_expr(parser)));
                switch (parser_next(parser)->kind) {
                    case TOKEN_COMMA: break;
                    case TOKEN_CLOSING_FIGURE_BRACE:
                        parser_skip_next(parser);
                        break;
                    default:
                        PARSE_ERROR(EXPECTED("comma or }"));
                        return NULL;
                }
            }
            return ast_expr_new_array(loc, values);
        }
        default:
            PARSE_ERROR(EXPECTED("expression"));
            return NULL;
    }
}

AstExpr *parse_expr_begin(Parser *parser) {
    Token *token = parser_next(parser);
    FileLocation loc = token->location;
    switch (token->kind) {
        case TOKEN_MINUS: return ast_expr_new_unary(loc, ast_expr_unary_new_arithmetic(
            NOT_NULL(parse_expr(parser)), AST_EXPR_UNARY_ARITHMETIC_MINUS
        ));
        case TOKEN_NOT: return ast_expr_new_unary(loc, ast_expr_unary_new_bool(
            NOT_NULL(parse_expr(parser)), AST_EXPR_UNARY_BOOL_NOT
        ));
        case TOKEN_BITAND: return ast_expr_new_unary(loc, ast_expr_unary_new_ref(
            NOT_NULL(parse_expr(parser))
        ));
        default:
            parser_skip_next(parser);
            return parse_expr_middle(parser);
    }
}

int ast_binop_priority(AstBinopKind kind) {
    switch (kind) {
        case AST_BINOP_AND: return 0;
        case AST_BINOP_OR: return 0;

        case AST_BINOP_EQ: return 100;
        case AST_BINOP_NEQ: return 100;
        case AST_BINOP_GT: return 100;
        case AST_BINOP_GE: return 100;
        case AST_BINOP_LT: return 100;
        case AST_BINOP_LE: return 100;

        case AST_BINOP_ADD: return 200;
        case AST_BINOP_SUB: return 200;

        case AST_BINOP_BITAND: return 300;
        case AST_BINOP_BITOR: return 300;
        case AST_BINOP_SHR: return 300;
        case AST_BINOP_SHL: return 300;
        case AST_BINOP_XOR: return 300;

        case AST_BINOP_MOD: return 400;
        case AST_BINOP_MUL: return 400;
        case AST_BINOP_DIV: return 400;
    }
    assert(0, "invalid binop kind");
}

void ast_expr_swap_binop_lprioritized(AstExpr *expr) {
    if (expr->kind != AST_EXPR_BINOP || expr->binop.left->kind != AST_EXPR_BINOP) {
        return;
    }
    AstExpr *left = expr->binop.left;
    AstExpr *right = expr->binop.right;
    int leftp = ast_binop_priority(left->binop.kind);
    int middlep = ast_binop_priority(expr->binop.kind);
    if (leftp < middlep) {
        // (a == c) + d
        // a == (c+d)
        AstBinopKind binopk = expr->binop.kind;
        AstExpr *ll = left->binop.left;
        AstExpr *lr = left->binop.right;
        AstExpr *r = right;
        expr->binop.kind = left->binop.kind;
        expr->binop.left = ll;
        expr->binop.right = left;
        left->binop.kind = binopk;
        left->binop.left = lr;
        left->binop.right = r;
        ast_expr_swap_binop_lprioritized(ll);
    }
}

AstExpr *ast_expr_binop_prioritized(AstExpr *left, AstExpr *right, FileLocation bloc, AstBinopKind binop) {
    AstExpr *expr = ast_expr_new_binop(left->loc, bloc, left, right, binop);
    while (right->kind == AST_EXPR_BINOP) {
        AstBinopKind kind = right->binop.kind;
        right->binop.kind = expr->binop.kind;
        expr->binop.kind = kind;
        expr->binop.right = right->binop.left;
        right = right->binop.right;
    }
    ast_expr_swap_binop_lprioritized(expr);
    return expr;
}

AstExpr *parse_expr_end(Parser *parser, AstExpr *inner) {
    Token *token = parser_next(parser);
    FileLocation loc = token->location;
    switch (token->kind) {
        case TOKEN_OPENING_SQUARE_BRACE: {
            AstExpr *idx = NOT_NULL(parse_expr(parser));
            PARSER_EXPECT_NEXT(TOKEN_CLOSING_SQUARE_BRACE, "]");
            return ast_expr_new_idx(loc, inner, idx);
        }
        case TOKEN_OPENING_CIRCLE_BRACE: {
            AstExpr **args = vec_new(AstExpr*);
            while (parser_next_is_not(parser, TOKEN_CLOSING_CIRCLE_BRACE)) {
                args = vec_push_dir(args, NOT_NULL(parse_expr(parser)));
                switch (parser_next(parser)->kind) {
                    case TOKEN_COMMA: break;
                    case TOKEN_CLOSING_CIRCLE_BRACE:
                        parser_skip_next(parser);
                        break;
                    default:
                        PARSE_ERROR(EXPECTED("comma or )"));
                        return NULL;
                }
            }
            return ast_expr_new_call(loc, inner, args);
        }
        case TOKEN_AS: {
            Token *token = parser_next_is_or(parser, TOKEN_AUTO);
            if (token) {
                return ast_expr_new_as_auto(loc, loc, inner, token->location);
            } else {
                return ast_expr_new_as_type(loc, loc, inner, NOT_NULL(parse_type(parser)));
            }
        }
        case TOKEN_QUESTION_MARK: return ast_expr_new_ret_on_null(loc, inner);
        case TOKEN_DOT: return ast_expr_new_inner_path(loc, inner, NOT_NULL(parse_path(parser)));
        case TOKEN_UNWRAP: return ast_expr_new_unwrap(loc, inner, PARSER_EXPECT_NEXT(TOKEN_IDENT, "variable name")->ident);
        case TOKEN_AND: return ast_expr_binop_prioritized(inner, NOT_NULL(parse_expr(parser)), loc, AST_BINOP_AND);
        case TOKEN_MOD: return ast_expr_binop_prioritized(inner, NOT_NULL(parse_expr(parser)), loc, AST_BINOP_MOD);
        case TOKEN_ADD: return ast_expr_binop_prioritized(inner, NOT_NULL(parse_expr(parser)), loc, AST_BINOP_ADD);
        case TOKEN_MINUS: return ast_expr_binop_prioritized(inner, NOT_NULL(parse_expr(parser)), loc, AST_BINOP_SUB);
        case TOKEN_MULTIPLY: return ast_expr_binop_prioritized(inner, NOT_NULL(parse_expr(parser)), loc, AST_BINOP_MUL);
        case TOKEN_DIVIDE: return ast_expr_binop_prioritized(inner, NOT_NULL(parse_expr(parser)), loc, AST_BINOP_DIV);
        case TOKEN_EQUALS: return ast_expr_binop_prioritized(inner, NOT_NULL(parse_expr(parser)), loc, AST_BINOP_EQ);
        case TOKEN_NOT_EQUALS: return ast_expr_binop_prioritized(inner, NOT_NULL(parse_expr(parser)), loc, AST_BINOP_NEQ);
        case TOKEN_GREATER: return ast_expr_binop_prioritized(inner, NOT_NULL(parse_expr(parser)), loc, AST_BINOP_GT);
        case TOKEN_GREATER_OR_EQUALS: return ast_expr_binop_prioritized(inner, NOT_NULL(parse_expr(parser)), loc, AST_BINOP_GE);
        case TOKEN_LESS: return ast_expr_binop_prioritized(inner, NOT_NULL(parse_expr(parser)), loc, AST_BINOP_LT);
        case TOKEN_LESS_OR_EQUALS: return ast_expr_binop_prioritized(inner, NOT_NULL(parse_expr(parser)), loc, AST_BINOP_LE);
        case TOKEN_OR: return ast_expr_binop_prioritized(inner, NOT_NULL(parse_expr(parser)), loc, AST_BINOP_OR);
        case TOKEN_BITAND: return ast_expr_binop_prioritized(inner, NOT_NULL(parse_expr(parser)), loc, AST_BINOP_BITAND);
        case TOKEN_BITOR: return ast_expr_binop_prioritized(inner, NOT_NULL(parse_expr(parser)), loc, AST_BINOP_BITOR);
        case TOKEN_SHR: return ast_expr_binop_prioritized(inner, NOT_NULL(parse_expr(parser)), loc, AST_BINOP_SHR);
        case TOKEN_SHL: return ast_expr_binop_prioritized(inner, NOT_NULL(parse_expr(parser)), loc, AST_BINOP_SHL);
        case TOKEN_XOR: return ast_expr_binop_prioritized(inner, NOT_NULL(parse_expr(parser)), loc, AST_BINOP_XOR);
        default:
            parser_skip_next(parser);
            return NULL;
    }
}

AstExpr *parse_expr(Parser *parser) {
    AstExpr *expr = NOT_NULL(parse_expr_begin(parser)); 
    AstExpr *result = NULL;
    while ((result = parse_expr_end(parser, expr))) expr = result;
    return expr;
}

