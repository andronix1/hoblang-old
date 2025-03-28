#pragma once

#include "ast/node/decl/from_use.h"
#include "sema/interface/module.h"

void sema_module_read_from_use(SemaModule *sema, AstFromUse *from_use, bool public);
