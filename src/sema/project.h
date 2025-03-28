#pragma once

#include <malloc.h>
#include "core/vec.h"
#include "sema/arch/info.h"
#include "sema/interface/module.h"

typedef struct {
    Slice path;
    SemaModule *module;
} SemaProjectModule;

typedef struct SemaProject {
    SemaProjectModule *modules;
    SemaArchInfo arch_info;
} SemaProject;

static inline SemaProjectModule sema_project_module_new(Slice path, SemaModule *module) {
    SemaProjectModule result = {
        .path = path,
        .module = module
    };
    return result;
}

static inline SemaProject *sema_project_new(SemaArchInfo arch_info) {
    SemaProject *result = malloc(sizeof(SemaProject));
    result->modules = vec_new(SemaProjectModule);
    result->arch_info = arch_info;
    return result;
}

SemaModule *sema_project_append_module_at(SemaProject *project, const char *path);
