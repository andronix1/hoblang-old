#pragma once

#include "sema/module.h"
#include "core/slice.h"

typedef struct SemaProjectModule {
    SemaModule *module;
    Slice path;
} SemaProjectModule;

typedef struct SemaProject {
    SemaProjectModule **modules;
} SemaProject;

