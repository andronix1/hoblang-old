#include "lexer/token.h"

void print_token(FILE *stream, va_list list) {	
	const Token *token = va_arg(list, Token*);
	if (token == NULL || token->type == TOKEN_EOI) {
		fprintf(stream, "<EOF>");
		return;
	}
	switch (token->type) {
    	case TOKEN_EOI: fprintf(stream, "<EOI>"); return;
        case TOKEN_ASSIGN_MOD: fprintf(stream, "%%="); return;
        case TOKEN_ASSIGN_ADD: fprintf(stream, "+="); return;
        case TOKEN_ASSIGN_MINUS: fprintf(stream, "-="); return;
        case TOKEN_ASSIGN_MULTIPLY: fprintf(stream, "*="); return;
        case TOKEN_ASSIGN_DIVIDE: fprintf(stream, "/="); return;
    	case TOKEN_ASM: fprintf(stream, "asm"); return;
    	case TOKEN_BREAK: fprintf(stream, "break"); return;
    	case TOKEN_CONTINUE: fprintf(stream, "continue"); return;
    	case TOKEN_AUTO: fprintf(stream, "_"); return;
    	case TOKEN_UNWRAP: fprintf(stream, "unwrap"); return;
    	case TOKEN_SIZEOF: fprintf(stream, "sizeof"); return;
    	case TOKEN_CONST: fprintf(stream, "const"); return;
    	case TOKEN_VOLATILE: fprintf(stream, "volatile"); return;
    	case TOKEN_DOLLAR: fprintf(stream, "$"); return;
    	case TOKEN_EXTERN: fprintf(stream, "extern"); return;
    	case TOKEN_NULL: fprintf(stream, "null"); return;
    	case TOKEN_TYPE: fprintf(stream, "type"); return;
    	case TOKEN_RETURN: fprintf(stream, "return"); return;
    	case TOKEN_FUNC_RETURNS: fprintf(stream, "->"); return;
    	case TOKEN_QUESTION_MARK: fprintf(stream, "?"); return;
    	case TOKEN_IMPORT: fprintf(stream, "import"); return;
    	case TOKEN_DEFER: fprintf(stream, "defer"); return;
    	case TOKEN_USE: fprintf(stream, "use"); return;
    	case TOKEN_AS: fprintf(stream, "as"); return;
    	case TOKEN_STRUCT: fprintf(stream, "struct"); return;
    	case TOKEN_FUN: fprintf(stream, "fun"); return;
    	case TOKEN_VAR: fprintf(stream, "var"); return;
		case TOKEN_IF: fprintf(stream, "if"); return;
    	case TOKEN_ELSE: fprintf(stream, "else"); return;
    	case TOKEN_WHILE: fprintf(stream, "while"); return;
    	case TOKEN_TRUE: fprintf(stream, "true"); return;
    	case TOKEN_FALSE: fprintf(stream, "false"); return;
		case TOKEN_COLON: fprintf(stream, ":"); return;
		case TOKEN_DOUBLE_COLON: fprintf(stream, "::"); return;
    	case TOKEN_COMMA: fprintf(stream, ","); return;
    	case TOKEN_SEMICOLON: fprintf(stream, ";"); return;
    	case TOKEN_ASSIGN: fprintf(stream, "="); return;
		case TOKEN_OPENING_CIRCLE_BRACE: fprintf(stream, "("); return;
		case TOKEN_CLOSING_CIRCLE_BRACE: fprintf(stream, ")"); return;
		case TOKEN_OPENING_FIGURE_BRACE: fprintf(stream, "{"); return;
		case TOKEN_CLOSING_FIGURE_BRACE: fprintf(stream, "}"); return;
		case TOKEN_OPENING_SQUARE_BRACE: fprintf(stream, "["); return;
		case TOKEN_CLOSING_SQUARE_BRACE: fprintf(stream, "]"); return;
		case TOKEN_NOT: fprintf(stream, "!"); return;
		case TOKEN_MOD: fprintf(stream, "%"); return;
		case TOKEN_BITNOT: fprintf(stream, "~"); return;
		case TOKEN_BITAND: fprintf(stream, "&"); return;
		case TOKEN_BITOR: fprintf(stream, "|"); return;
		case TOKEN_XOR: fprintf(stream, "^"); return;
		case TOKEN_AND: fprintf(stream, "&&"); return;
		case TOKEN_OR: fprintf(stream, "||"); return;
		case TOKEN_DOT: fprintf(stream, "."); return;
		case TOKEN_ADD: fprintf(stream, "+"); return;
    	case TOKEN_MINUS: fprintf(stream, "-"); return;
    	case TOKEN_MULTIPLY: fprintf(stream, "*"); return;
    	case TOKEN_DIVIDE: fprintf(stream, "/"); return;
    	case TOKEN_EQUALS: fprintf(stream, "=="); return;
    	case TOKEN_NOT_EQUALS: fprintf(stream, "!="); return;
    	case TOKEN_LESS: fprintf(stream, "<"); return;
    	case TOKEN_LESS_OR_EQUALS: fprintf(stream, "<="); return;
    	case TOKEN_SHL: fprintf(stream, "<<"); return;
    	case TOKEN_GREATER: fprintf(stream, ">"); return;
    	case TOKEN_GREATER_OR_EQUALS: fprintf(stream, ">="); return;
    	case TOKEN_SHR: fprintf(stream, ">>"); return;
    	case TOKEN_INTEGER: fprintf(stream, "integer %ld", token->integer); return;
    	case TOKEN_FLOAT: fprintf(stream, "float %Lf", token->float_value); return;
    	case TOKEN_CHAR: fprintf(stream, "char '%c'", token->character); return;
    	case TOKEN_STR: {
			Slice str = {
				.len = vec_len(token->str),
				.str = token->str
			};
			print_to(stream, "string \"{slice}\"", &str);
			return;
		}
    	case TOKEN_IDENT: 
			fprintf(stream, "ident `");
			slice_write_to(&token->ident, stream);
			fprintf(stream, "`");
			return;
	}
	fprintf(stream, "<unknown token id %x>", token->type);
}

