#include "tokens.h"

TokenType token_type(Token *token) {
	return token == NULL ? TOKEN_EOI : token->type;
}

void print_token(FILE *stream, va_list *list) {	
	const Token *token = va_arg(*list, Token*);
	if (token == NULL || token->type == TOKEN_EOI) {
		fprintf(stream, "<EOF>");
		return;
	}
	switch (token->type) {
    	case TOKEN_EXTERN: fprintf(stream, "extern"); break;
    	case TOKEN_MODULE: fprintf(stream, "module"); break;
    	case TOKEN_AS: fprintf(stream, "as"); break;
    	case TOKEN_FUN: fprintf(stream, "fun"); break;
    	case TOKEN_VAR: fprintf(stream, "var"); break;
		case TOKEN_IF: fprintf(stream, "if"); break;
    	case TOKEN_ELSE: fprintf(stream, "else"); break;
    	case TOKEN_WHILE: fprintf(stream, "while"); break;
    	case TOKEN_TRUE: fprintf(stream, "true"); break;
    	case TOKEN_FALSE: fprintf(stream, "false"); break;
		case TOKEN_COLON: fprintf(stream, ":"); break;
    	case TOKEN_COMMA: fprintf(stream, ","); break;
    	case TOKEN_SEMICOLON: fprintf(stream, ";"); break;
    	case TOKEN_ASSIGN: fprintf(stream, "="); break;
		case TOKEN_OPENING_CIRCLE_BRACE: fprintf(stream, "("); break;
		case TOKEN_CLOSING_CIRCLE_BRACE: fprintf(stream, ")"); break;
		case TOKEN_OPENING_FIGURE_BRACE: fprintf(stream, "{"); break;
		case TOKEN_CLOSING_FIGURE_BRACE: fprintf(stream, "}"); break;
		case TOKEN_ADD: fprintf(stream, "+"); break;
    	case TOKEN_MINUS: fprintf(stream, "-"); break;
    	case TOKEN_MULTIPLY: fprintf(stream, "*"); break;
    	case TOKEN_DIVIDE: fprintf(stream, "/"); break;
    	case TOKEN_EQUALS: fprintf(stream, "=="); break;
    	case TOKEN_NOT_EQUALS: fprintf(stream, "!="); break;
    	case TOKEN_LESS: fprintf(stream, "<"); break;
    	case TOKEN_LESS_OR_EQUALS: fprintf(stream, "<="); break;
    	case TOKEN_GREATER: fprintf(stream, ">"); break;
    	case TOKEN_GREATER_OR_EQUALS: fprintf(stream, ">="); break;
    	case TOKEN_INTEGER: fprintf(stream, "integer %ld", token->integer); break;
    	case TOKEN_CHAR: fprintf(stream, "char '%c'", token->character); break;
    	case TOKEN_STR: fprintf(stream, "string \"TODO\""); break;
    	case TOKEN_IDENT: 
			fprintf(stream, "ident `");
			slice_write_to(&token->ident, stream);
			fprintf(stream, "`");
			break;
		default: fprintf(stream, "<unknown token id %lx>", token->type); break;
	}
}
