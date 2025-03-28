#pragma once

#include "sema/arch/info.h"
#include <stdbool.h>

typedef struct SemaProject SemaProject;

SemaProject *sema_project_from_file(const char *path, SemaArchInfo arch_info);
bool sema_project_analyze(SemaProject *project);
