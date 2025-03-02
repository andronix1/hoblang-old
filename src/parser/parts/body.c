#include "ast/private/body.h"
#include "ast/private/stmts/loop_control.h"
#include "ast/private/expr/binop.h"
#include "ast/private/expr.h"
#include "core/location.h"
#include "lexer/token.h"
#include "parser/parts/expr.h"
#include "parser/private.h"
#include "parser/token_stops.h"

bool parse_loop_control(Parser *parser, AstStmtLoopControl *loop_control);
bool parse_defer(Parser *parser, AstDefer *defer);
bool parse_if_else(Parser *parser, AstIfElse *if_else);
bool parse_return(Parser *parser, AstReturn *ret);
bool parse_var(Parser *parser, AstVar *var);
bool parse_while(Parser *parser, AstWhile *while_loop);
bool parse_asm_body(Parser *parser, AstInlineAsm *inline_asm);

static inline bool token_stop_stmt(TokenType type) {
    return
        type == TOKEN_ASSIGN ||
        type == TOKEN_ASSIGN_MOD || 
        type == TOKEN_ASSIGN_ADD || 
        type == TOKEN_ASSIGN_MINUS || 
        type == TOKEN_ASSIGN_MULTIPLY || 
        type == TOKEN_ASSIGN_DIVIDE || 
        type == TOKEN_SEMICOLON;
}

inline static bool parse_assign(Parser *parser, AstExpr *into, AstStmt *stmt) {
    stmt->type = AST_STMT_ASSIGN;
    stmt->assign.assign_expr = into;
    stmt->assign.loc = parser_token(parser)->location;
    return (stmt->assign.expr = parse_expr(parser, token_stop_semicolon));
}

inline static bool parse_assign_wrapped(Parser *parser, AstExpr *into, AstStmt *stmt, AstBinopType type) {
    if (!parse_assign(parser, into, stmt)) {
        return false;
    }
    stmt->assign.expr = ast_expr_binop(
        stmt->assign.loc,
        type,
        into,
        stmt->assign.expr
    );
    return true;
}

bool parse_stmt(Parser *parser, AstStmt *stmt) {
	Token *token = parser_next(parser);
	switch (token->type) {
		case TOKEN_IDENT:
		case TOKEN_OPENING_CIRCLE_BRACE: {
			parser_skip_next(parser);
			AstExpr *expr = parse_expr(parser, token_stop_stmt);
			if (!expr) {
				return false;
			}
			parser_step(parser);
			switch (token->type) {
				case TOKEN_ASSIGN:
					return parse_assign(parser, expr, stmt);
                case TOKEN_ASSIGN_MOD:
                    return parse_assign_wrapped(parser, expr, stmt, AST_BINOP_MOD); 
                case TOKEN_ASSIGN_ADD:
                    return parse_assign_wrapped(parser, expr, stmt, AST_BINOP_ADD); 
                case TOKEN_ASSIGN_MINUS:
                    return parse_assign_wrapped(parser, expr, stmt, AST_BINOP_SUB); 
                case TOKEN_ASSIGN_MULTIPLY:
                    return parse_assign_wrapped(parser, expr, stmt, AST_BINOP_MUL); 
                case TOKEN_ASSIGN_DIVIDE:
                    return parse_assign_wrapped(parser, expr, stmt, AST_BINOP_DIV); 
				case TOKEN_SEMICOLON:
					stmt->type = AST_STMT_EXPR;
					stmt->expr = expr;
					return true;
				default:
					PARSE_ERROR("unexpected token `{tok}` after expression in statement", token);
					return false;
			}
		}
		case TOKEN_CONTINUE:
            stmt->type = AST_STMT_CONTINUE;
			stmt->continue_loop.loc = token->location;
            return parse_loop_control(parser, &stmt->continue_loop);
		case TOKEN_BREAK:
            stmt->type = AST_STMT_BREAK;
			stmt->break_loop.loc = token->location;
            return parse_loop_control(parser, &stmt->break_loop);
		case TOKEN_DEFER:
			stmt->type = AST_STMT_DEFER;
			return parse_defer(parser, &stmt->defer);
		case TOKEN_IF:
			stmt->type = AST_STMT_IF;
			return parse_if_else(parser, &stmt->if_else);
		case TOKEN_RETURN:
			stmt->type = AST_STMT_RETURN;
			stmt->ret.loc = token->location;
			return parse_return(parser, &stmt->ret);
		case TOKEN_WHILE:
			stmt->type = AST_STMT_WHILE;
			stmt->while_loop.loc = token->location;
			return parse_while(parser, &stmt->while_loop);
		case TOKEN_VAR:
			stmt->type = AST_STMT_VAR;
			stmt->var.loc = token->location;
			return parse_var(parser, &stmt->var);
		case TOKEN_ASM:
			stmt->type = AST_STMT_INLINE_ASM;
			return parse_asm_body(parser, &stmt->inline_asm);
		default:
			PARSE_ERROR("unexpected `{tok}` while parsing statement", parser_token(parser));
			return false;
	}
}

bool parse_body_maybe_ola(Parser *parser, AstBody *body, bool one_line_allowed) {
	body->stmts = vec_new(AstStmt);
	parser_step(parser);
	switch (parser_token(parser)->type) {
		case TOKEN_OPENING_FIGURE_BRACE: break;
		default: {
			parser_skip_next(parser);
			if (!one_line_allowed) {
				PARSE_ERROR(EXPECTED("body open"));
				return false;
			}
			AstStmt stmt;
			if (parse_stmt(parser, &stmt)) {
				body->stmts = vec_push(body->stmts, &stmt);
				return true;
			}
			return false;
		}
	}
	while (true) {
		parser_step(parser);
		switch (parser_token(parser)->type) {
			case TOKEN_SEMICOLON:
				break;
			case TOKEN_CLOSING_FIGURE_BRACE:
				return true;
			case TOKEN_EOI:
				PARSE_ERROR("EOI while parsing body");
				return false;
			default: {
				parser_skip_next(parser);
				AstStmt stmt;
				if (parse_stmt(parser, &stmt)) {
					body->stmts = vec_push(body->stmts, &stmt);
				}
				break;
			}
		}
	}
	PARSER_EXPECT_NEXT(TOKEN_CLOSING_FIGURE_BRACE, "body closing");
	return false;
}

bool parse_body_ola(Parser *parser, AstBody *body) {
	return parse_body_maybe_ola(parser, body, true);
}

bool parse_body(Parser *parser, AstBody *body) {
	return parse_body_maybe_ola(parser, body, false);
}
