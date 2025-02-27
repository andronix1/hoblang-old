#pragma once

#include "sema/arch/bits/bits.h"
#include "sema/module.h"
#include "core/slice.h"

typedef struct SemaProjectModule {
    SemaModule *module;
    Slice path;
} SemaProjectModule;

typedef struct SemaProject {
    SemaProjectModule **modules;
    SemaArchInfo arch_info;
} SemaProject;

