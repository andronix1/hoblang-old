#pragma once

#include <stdbool.h>
#include "project.h"

typedef struct SemaModule SemaModule;

SemaModule *sema_module_from_file(const char *path, SemaProject *project);
bool sema_module_read(SemaModule *sema);
bool sema_module_analyze(SemaModule *sema);
