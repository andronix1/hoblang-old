#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

typedef struct SemaType SemaType;
typedef struct SemaFunction SemaFunction;

bool sema_types_equals(SemaType *type, SemaType *other);
void print_sema_type(FILE *stream, va_list list);
