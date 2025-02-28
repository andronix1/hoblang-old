#pragma once

#include <stdbool.h>
#include "sema/module.h"
#include "sema/type/private.h"
#include "sema/value/value.h"

SemaType *sema_value_typeof(SemaValue *value);
bool sema_value_is_type_of(SemaValue *value, SemaTypeKind kind);
SemaModule *sema_value_module_of(SemaValue *value);
