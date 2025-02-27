#pragma once

#include <stdbool.h>
#include "sema/arch/bits/bits.h"
#include "sema/project.h"
#include "sema/module.h"
#include "sema/project/module.h"
#include "core/slice.h"

SemaProject *sema_project_new(SemaArchInfo arch_info);
SemaModule *sema_project_add_module_at(SemaProject *project, const char *path);
bool sema_project_analyze(SemaProject *project);

SemaProjectModule **sema_project_modules(SemaProject *project);
SemaModule *sema_project_module_inner(SemaProjectModule *module);
Slice sema_project_module_path(SemaProjectModule *module);
