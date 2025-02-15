#pragma once

#include <stdbool.h>
#include "type.h"

typedef struct SemaFunction SemaFunction;

bool sema_types_equals(SemaType *type, SemaType *other);
bool sema_type_is_int(SemaType *type);
