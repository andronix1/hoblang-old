#pragma once

#include "ast/interface/global.h"
#include "sema/interface/module.h"

void sema_module_read_global_decl_info(SemaModule *sema, AstGlobalDeclInfo *info, bool public);
