#pragma once

#include "core/location.h"
#include "core/slice/slice.h"
#include "sema/module/behaviour/behaviour.h"
#include "sema/module/module.h"
#include "sema/type/private.h"
#include "sema/value.h"

bool sema_type_behaves_as(SemaModule *sema, FileLocation at, SemaType *type, SemaBehaviour *behaviour);
SemaValue *sema_behaviour_resolve(SemaBehaviour *behaviour, Slice *name);
SemaType *sema_behaviour_swap_self_type(SemaBehaviour *behaviour, SemaType *new);
