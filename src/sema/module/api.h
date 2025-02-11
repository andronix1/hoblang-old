#pragma once

#include "module.h"
#include "sema/project/project.h"
#include "ast/api.h"

SemaModule *sema_module_new(SemaProject *project, AstModule *module);
AstModule *sema_module_of(SemaModule *module);
void sema_module_read_decls(SemaModule *sema);
void sema_module_analyze(SemaModule *sema);
bool sema_module_failed(const SemaModule *sema);

SemaProject *sema_module_project(const SemaModule *sema);
