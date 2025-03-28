#pragma once

#include "ast/node/decl/behaviour.h"
#include "sema/interface/module.h"

bool sema_module_read_behaviour(SemaModule *sema, AstBehaviourDecl *decl, bool is_generic, bool public);
