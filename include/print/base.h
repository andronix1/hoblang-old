#pragma once

#include <stdio.h>
#include <stdarg.h>

void print_slice(FILE *stream, va_list list);
void print_errno(FILE *stream, va_list list);
void print_bool(FILE *stream, va_list list);
void print_size(FILE *stream, va_list list);
void print_int(FILE *stream, va_list list);
void print_char(FILE *stream, va_list list);
void print_cstr(FILE *stream, va_list list);
