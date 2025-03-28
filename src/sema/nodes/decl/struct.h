#pragma once

#include "ast/node/decl/struct.h"
#include "sema/interface/module.h"

bool sema_module_read_struct(SemaModule *sema, AstStructDecl *decl, bool is_generic, bool public);
