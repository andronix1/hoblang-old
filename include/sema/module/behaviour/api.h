#pragma once

#include "core/slice/slice.h"
#include "sema/module/behaviour/behaviour.h"
#include "sema/type/private.h"
#include "sema/value.h"
#include "table/path.h"

SemaValue *sema_behaviour_resolve(SemaBehaviour *behaviour, Slice *name, SemaBehaviourTablePath *result);
SemaType *sema_behaviour_swap_self_type(SemaBehaviour *behaviour, SemaType *new);
