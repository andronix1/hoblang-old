#pragma once

#include <stdio.h>
#include <stddef.h>

void print_ast_mod_path(FILE *stream, va_list list);
void print_ast_binop_type(FILE *stream, va_list list);
void print_ast_type(FILE *stream, va_list list);
void print_ast_path(FILE *stream, va_list list);
