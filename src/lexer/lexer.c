#include "lexer.h"

bool lexer_parse_type(Lexer *lexer, AstType *to) {
	Token *type_token = lexer_next_token(lexer);
	if (token_type(type_token) != TOKEN_IDENT) {
		lex_error("expected type found `%T`", type_token);
		return false;
	}
	to->name = type_token;
	return true;
}

typedef enum {
	LEX_STMT_OK = 0,
	LEX_STMT_ERR,
	LEX_STMT_MISSMATCH
} LexStmtErr;

LexStmtErr lex_stmt_let(Lexer *lexer, AstStmts *stmts) {
	if (token_type(lexer_next_token(lexer)) != TOKEN_LET) return LEX_STMT_MISSMATCH;
	Token *name = lexer_next_token(lexer);
	if (token_type(name) != TOKEN_IDENT) {
		lex_error("expected variable name found `%T`", name);
		return LEX_STMT_ERR;
	}
	if (token_type(lexer_future_token(lexer)) != TOKEN_ASSIGN) {
		return LEX_STMT_OK;
	}
	lexer_next_token(lexer);
	Token *first_token = lexer_future_token(lexer);
	size_t len = lexer_find_scoped(lexer, TOKEN_SEMICOLON);
	if (len == 0) {
		lex_error("expected expression");
		return LEX_STMT_ERR;
	}
	TokensSlice expr_tokens = { first_token, len };
	Expr expr;
	if (!expr_parse(lexer, &expr, &expr_tokens)) {
		return false;
	}
	return LEX_STMT_OK;
}

#define LEX_STMT(parse) \
	do { \
		if ((err = parse(lexer, &block->stmts)) == LEX_STMT_MISSMATCH) { \
			lexer->state = state; \
		} else { \
			goto success; \
		} \
	} while(0)

bool lexer_parse_code_block(Lexer *lexer, AstCodeBlock *block) {
	Token *token = lexer_future_token(lexer);
	while (token_type(token) != TOKEN_CLOSING_FIGURE_BRACE && token_type(token) != TOKEN_EOF) {
		LexStmtErr err;
		LexerState state = lexer->state;

		LEX_STMT(lex_stmt_let);

		lex_error("unknown statement starts with `%T`!", token);
		success:
		token = lexer_next_token(lexer);
		if (token_type(token) != TOKEN_SEMICOLON) {
			if (!err) {
				lex_error("expected semicolon found `%T`", token);
			}
			while (true) {
				token = lexer_next_token(lexer);
				if (token_type(token) == TOKEN_EOF) {
					return false;
				} else if (token_type(token) == TOKEN_SEMICOLON) {
					break;
				}
			}
		}
		token = lexer_future_token(lexer);
	}
	return true;
}

bool lexer_parse_fun(Lexer *lexer) {
	Token *name = lexer_next_token(lexer);
	if (token_type(name) != TOKEN_IDENT) {
		lex_error("expected function name but found `%T`", name);
		return false;
	}
	Token *opening_args_brace = lexer_next_token(lexer);
	if (token_type(opening_args_brace) != TOKEN_OPENING_CIRCLE_BRACE) {
		lex_error("expected args opening brace but found `%T`", opening_args_brace);
		return false;
	}
	bool reading_args = true;
	Token *next = lexer_next_token(lexer);
	while (reading_args) {
		switch (token_type(next)) {
			case TOKEN_CLOSING_CIRCLE_BRACE:
				reading_args = false;
				break;
			case TOKEN_IDENT: {
				next = lexer_next_token(lexer);
				if (token_type(next) != TOKEN_COLON) {
					lex_error("expected arg separation colon but got `%T`", next);
					return false;
				}
				AstType type;
				if (!lexer_parse_type(lexer, &type)) {
					return false;
				}
				next = lexer_next_token(lexer);
				if (token_type(next) == TOKEN_COMMA) {
					next = lexer_next_token(lexer);
				} else if (token_type(next) != TOKEN_CLOSING_CIRCLE_BRACE) {
					lex_error("expected arg break but got `%T`", next);
					return false;
				}
				break;
			}
			default:
				lex_error("expected arg or args closing brace but got `%T`", next);
				return false;
		}
	}
	Token *opening_body_brace = NULL;
	Token *colon = opening_body_brace = lexer_next_token(lexer);
	if (token_type(colon) == TOKEN_COLON) {
		AstType type;
		if (!lexer_parse_type(lexer, &type)) {
			return false;
		}
		opening_body_brace = lexer_next_token(lexer);
	} else {
		// func_decl.returning = false;
	}
	if (token_type(opening_body_brace) != TOKEN_OPENING_FIGURE_BRACE) {
		lex_error("expected body found %T", opening_body_brace);
		return false;
	}
	AstCodeBlock block;
	if (!lexer_parse_code_block(lexer, &block)) {
		return false;
	}
	return true;
}

bool lexer_parse(Lexer *lexer) {
	Token *token = lexer_next_token(lexer);
	switch (token_type(token)) {
		case TOKEN_FUN: return lexer_parse_fun(lexer);
		default:
			lex_error("unexpected token: `%T`!", token);
			return false;
	}
}
