#include "ast/interface/expr.h"
#include <stdarg.h>
#include <stdio.h>

void print_ast_binop_type(FILE *stream, va_list list) {
	AstBinopKind kind = va_arg(list, AstBinopKind);
    switch (kind) {
        case AST_BINOP_AND: fprintf(stream, "&&"); break;
        case AST_BINOP_MOD: fprintf(stream, "%%"); break;
        case AST_BINOP_ADD: fprintf(stream, "+"); break;
        case AST_BINOP_SUB: fprintf(stream, "-"); break;
        case AST_BINOP_MUL: fprintf(stream, "*"); break;
        case AST_BINOP_DIV: fprintf(stream, "/"); break;
        case AST_BINOP_EQ: fprintf(stream, "=="); break;
        case AST_BINOP_NEQ: fprintf(stream, "!="); break;
        case AST_BINOP_GT: fprintf(stream, ">"); break;
        case AST_BINOP_GE: fprintf(stream, ">="); break;
        case AST_BINOP_LT: fprintf(stream, "<"); break;
        case AST_BINOP_LE: fprintf(stream, "<="); break;
        case AST_BINOP_OR: fprintf(stream, "||"); break;    
        case AST_BINOP_BITAND: fprintf(stream, "&"); break;
        case AST_BINOP_BITOR: fprintf(stream, "|"); break;
        case AST_BINOP_SHR: fprintf(stream, ">>"); break;
        case AST_BINOP_SHL: fprintf(stream, "<<"); break;
        case AST_BINOP_XOR: fprintf(stream, "^"); break;
    }
}
