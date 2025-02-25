#pragma once

#include <stdbool.h>
#include "lexer/lexer.h"
#include "core/slice.h"
#include "core/location.h"
#include "ast/api/module.h"
#include "sema/project.h"
#include "module.h"

SemaModule *sema_module_new(SemaProject *project, AstModule *module);
AstModule *sema_module_of(SemaModule *module);
void sema_module_read_decls(SemaModule *sema);
void sema_module_analyze(SemaModule *sema);
bool sema_module_failed(const SemaModule *sema);

SemaProject *sema_module_project(const SemaModule *sema);
