#include "expr.h"

bool expr_token_is_operator(TokenType type, ExprPartOpType *output) {
	switch (type) {
		case TOKEN_ADD: 
			*output = EXPR_PART_OP_ADD;
			return true;

		case TOKEN_MINUS: 
			*output = EXPR_PART_OP_MINUS;
			return true;

		case TOKEN_MULTIPLY: 
			*output = EXPR_PART_OP_MULTIPLY;
			return true;

		case TOKEN_DIVIDE: 
			*output = EXPR_PART_OP_DIVIDE;
			return true;

		default:
			return false;
	}
}

// TODO: fix memory leaks
bool expr_raw_parse(Lexer *lexer, RawExpr *output, TokensSlice *tokens) {
	ExprPart part;
	for (size_t i = 0; i < tokens->len; i++) {
		const Token *token = &tokens->tokens[i];
		if (expr_token_is_operator(token->type, &part.op_type)) {
			part.type = EXPR_PART_OPERATION;
			vec_push(output, &part);
		} else if (token->type == TOKEN_IDENT) {
			part.type = EXPR_PART_READY;
			part.ready.type = EXPR_IDENT;
			part.ready.ident = token->ident;
		} else if (token->type == TOKEN_INTEGER) {
			part.type = EXPR_PART_READY;
			part.ready.type = EXPR_INTEGER;
			part.ready.integer = token->integer;
		} else if (token->type == TOKEN_OPENING_CIRCLE_BRACE) {
			size_t len = 0;
			size_t level = 0;
			bool finished = false;
			for (i++; !finished && i < tokens->len; i++, len++) {
				const Token *jtoken = &tokens->tokens[i];	
				switch (jtoken->type) {
					case TOKEN_OPENING_CIRCLE_BRACE:
						level++;
						break;
					case TOKEN_CLOSING_CIRCLE_BRACE:
						if (level == 0) {
							finished = true;
							break;
						}
						level--;
						break;
					default: break;
				}
			}
			if (!finished) {
				lex_error("expression scope is unclosed");
				return false;
			}
			part.scope = vec_new(ExprPart);
			TokensSlice scope_tokens = { &tokens->tokens[i - len], len - 1 };
			if (!expr_raw_parse(lexer, &part.scope, &scope_tokens)) {
				return false;
			}
		} else {	
			lex_error_at(token->location, "unexpected token `%T` while parsing expression", token);
			return false;
		}
	}
	return true;
}

bool expr_from_raw_expr(Lexer *lexer, Expr *output, RawExpr *raw_expr) {

}

bool expr_parse(Lexer *lexer, Expr *output, TokensSlice *tokens) {
	RawExpr raw_expr = vec_new(ExprPart);
	if (!expr_raw_parse(lexer, &raw_expr, tokens)) {
		return false;
	}
	// TODO: raw expr to expr
	return true;
}
