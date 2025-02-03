#include "../parsers.h"

bool parse_defer(Parser *parser, AstDefer *defer);
bool parse_if_else(Parser *parser, AstIfElse *if_else);
bool parse_return(Parser *parser, AstReturn *ret);
bool parse_var(Parser *parser, AstVar *var);
bool parse_while(Parser *parser, AstWhile *while_loop);
bool parse_asm_body(Parser *parser, AstInlineAsm *inline_asm);

static inline bool token_stmt_stop(TokenType type) { return type == TOKEN_ASSIGN || type == TOKEN_SEMICOLON; }

bool parse_stmt(Parser *parser, AstStmt *stmt) {
	parser_next_token(parser);
	switch (token_type(parser->token)) {
		case TOKEN_IDENT:
		case TOKEN_OPENING_CIRCLE_BRACE: {
			parser->skip_next = true;
			AstExpr *expr = parse_expr(parser, token_stmt_stop);
			if (!expr) {
				return false;
			}
			parser_next_token(parser);
			switch (token_type(parser->token)) {
				case TOKEN_ASSIGN:
					stmt->type = AST_STMT_ASSIGN;
					stmt->assign.assign_expr = expr;
					return (stmt->assign.expr = parse_expr(parser, token_semicolon_stop));
				default:
					parse_err("unexpected token `{tok}` after expression in statement");
					return false;
			}
		}
		case TOKEN_DEFER:
			stmt->type = AST_STMT_DEFER;
			return parse_defer(parser, &stmt->defer);
		case TOKEN_IF:
			stmt->type = AST_STMT_IF;
			return parse_if_else(parser, &stmt->if_else);
		case TOKEN_RETURN:
			stmt->type = AST_STMT_RETURN;
			return parse_return(parser, &stmt->ret);
		case TOKEN_WHILE:
			stmt->type = AST_STMT_WHILE;
			return parse_while(parser, &stmt->while_loop);
		case TOKEN_VAR:
			stmt->type = AST_STMT_VAR;
			return parse_var(parser, &stmt->var);
		case TOKEN_ASM:
			stmt->type = AST_STMT_INLINE_ASM;
			return parse_asm_body(parser, &stmt->inline_asm);
		default:
			parse_err("unexpected `{tok}` while parsing statement", parser->token);
			return false;
	}
}

bool parse_body_maybe_ola(Parser *parser, AstBody *body, bool one_line_allowed) {
	body->stmts = vec_new(AstStmt);
	parser_next_token(parser);
	switch (token_type(parser->token)) {
		case TOKEN_OPENING_FIGURE_BRACE: break;
		default: {
			parser->skip_next = true;
			if (!one_line_allowed) {
				parse_err(EXPECTED("body open"));
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
					body->stmts = vec_push(body->stmts, &stmt);
				}
				break;
			}
		}
	}
	parse_exp_next(TOKEN_CLOSING_FIGURE_BRACE, "body closing");
	return false;
}

bool parse_body_ola(Parser *parser, AstBody *body) {
	return parse_body_maybe_ola(parser, body, true);
}

bool parse_body(Parser *parser, AstBody *body) {
	return parse_body_maybe_ola(parser, body, false);
}
