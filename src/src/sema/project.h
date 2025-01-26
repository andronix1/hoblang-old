#pragma once

#include <unistd.h>
#include <linux/limits.h>
#include "core/path.h"
#include "lexer/lexer.h"
#include "parser/parsers.h"
#include "module.h"

typedef struct {
    SemaModule *module;
    Slice path;
} SemaProjectModule;

typedef struct _SemaProject {
    SemaProjectModule *modules;
} SemaProject;

SemaProject *sema_project_new();
SemaModule *sema_project_add_module_at(SemaProject *project, const char *path);
bool sema_project_analyze(SemaProject *project);