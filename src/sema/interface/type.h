#pragma once

#include <stdbool.h>

typedef struct SemaType SemaType;

bool sema_types_equals(SemaType *type, SemaType *other);
bool sema_type_is_int(SemaType *type);
bool sema_type_is_float(SemaType *type);
