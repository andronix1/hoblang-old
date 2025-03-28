#include <malloc.h>
#include "project.h"
#include "core/vec.h"
#include "sema/interface/module.h"
#include "sema/project.h"

SemaProject *sema_project_from_file(const char *path, SemaArchInfo arch_info) {
    SemaProject *project = sema_project_new(arch_info);
    sema_project_append_module_at(project, path);
    return project;
}

bool sema_project_analyze(SemaProject *project) {
    bool succeed = true;
    for (size_t i = 0; i < vec_len(project->modules); i++) {
        if (!sema_module_analyze(project->modules[i].module)) {
            succeed = false;
        }
    }
    return succeed;
}
