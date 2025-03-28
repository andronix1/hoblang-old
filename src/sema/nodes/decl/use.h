#pragma once

#include "ast/node/decl/use.h"
#include "sema/interface/module.h"

void sema_module_read_use(SemaModule *sema, AstUse *use, bool public);
