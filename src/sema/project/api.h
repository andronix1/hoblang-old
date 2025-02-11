#pragma once

#include "project.h"
#include "sema/module/module.h"
#include "core/slice.h"

SemaProject *sema_project_new();
SemaModule *sema_project_add_module_at(SemaProject *project, const char *path);
bool sema_project_analyze(SemaProject *project);

SemaProjectModule **sema_project_modules(SemaProject *project);
SemaModule *sema_project_module_inner(SemaProjectModule *module);
Slice sema_project_module_path(SemaProjectModule *module);
