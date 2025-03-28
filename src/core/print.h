#pragma once

#include <stdarg.h>
#include <stdio.h>
#include "core/slice/slice.h"

typedef void(*PrintFunc)(FILE* stream, va_list args);

void print_init();
void print_va_list(const char *fmt, va_list args);
void print_register(Slice name, PrintFunc func);
void print_to(FILE *stream, const char *fmt, ...);
void print(const char *fmt, ...);
