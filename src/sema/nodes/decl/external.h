#pragma once

#include "ast/node/decl/external.h"
#include "sema/interface/module.h"

void sema_module_read_external(SemaModule *sema, AstExternalDecl *decl, bool public);
