#pragma once

#include "ast/node/decl/func.h"
#include "sema/interface/module.h"

void sema_module_read_func(SemaModule *sema, AstFuncDecl *decl, bool is_generic, bool public);
