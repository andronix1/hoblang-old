#pragma once

#include "core/location.h"
#include "sema/module/behaviour/behaviour.h"
#include "sema/module/module.h"
#include "sema/type/type.h"

typedef struct SemaBehaviourTable SemaBehaviourTable;

SemaBehaviourTable *sema_type_build_behaviour_table(SemaModule *sema, FileLocation at, SemaType *type, SemaBehaviour *behaviour);

