#pragma once

#include "ast/interface/global_decl.h"
#include "sema/interface/module.h"

void sema_module_read_global_decl(SemaModule *sema, AstGlobalDecl *global_decl, bool public);
void sema_module_analyze_global_decl(SemaModule *sema, AstGlobalDecl *global_decl);
