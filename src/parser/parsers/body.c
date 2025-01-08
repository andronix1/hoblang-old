#include "../parsers.h"

#include "stmts/if_else.c"
#include "stmts/var.c"
#include "stmts/return.c"

bool parse_stmt(Parser *parser, AstStmt *stmt) {
	parser_next_token(parser);
	switch (token_type(parser->token)) {
		case TOKEN_IDENT: {
			Slice name = parser->token->ident;
			parser_next_token(parser);
			switch (token_type(parser->token)) {
				case TOKEN_OPENING_CIRCLE_BRACE:
					stmt->type = AST_STMT_FUNC_CALL;
					stmt->func_call.name = name;
					return parse_func_call_args(parser, &stmt->func_call);
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
					body->stmts = vec_push(body->stmts, &stmt);
				}
				break;
			}
		}
	}
	parse_exp_next(TOKEN_CLOSING_FIGURE_BRACE, "body closing");
	return false;
}
