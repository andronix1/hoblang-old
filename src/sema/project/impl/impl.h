#pragma once

#include "sema/module/module.h"
#include "core/slice.h"
#include "../api.h"

typedef struct SemaProjectModule {
    SemaModule *module;
    Slice path;
} SemaProjectModule;

typedef struct SemaProject {
    SemaProjectModule **modules;
} SemaProject;

