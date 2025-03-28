#pragma once

#include "ast/node/decl/val.h"
#include "sema/interface/module.h"

void sema_module_read_val_decl(SemaModule *sema, AstValDecl *decl, bool public);
