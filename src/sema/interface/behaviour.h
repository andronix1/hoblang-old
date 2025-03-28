#pragma once

#include "sema/interface/type.h"
#include "sema/interface/module.h"

typedef struct SemaBehaviour SemaBehaviour;

bool sema_type_behaves_like(SemaModule *module, SemaType *type, SemaBehaviour *behaviour);
