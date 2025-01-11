#pragma once

#include <stdarg.h>
#include <stdio.h>

void print_ast_expr(FILE *stream, va_list *list);
void print_ast_binop_type(FILE *stream, va_list *list);
void print_ast_type(FILE *stream, va_list *list);
