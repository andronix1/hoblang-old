#pragma once

#include "sema/interface/const.h"
#include "sema/interface/module.h"
#include "sema/interface/type.h"

typedef struct SemaValue SemaValue;

SemaModule *sema_value_is_module(SemaValue *value);
SemaType *sema_value_is_type(SemaValue *value);
SemaType *sema_value_is_var(SemaValue *value);
SemaConst *sema_value_is_const(SemaValue *value);
SemaType *sema_value_is_runtime(SemaValue *value);
SemaValue *sema_runtime_value_nest_with(SemaValue *source, SemaType *type);
