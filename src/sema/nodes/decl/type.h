#pragma once

#include "ast/node/decl/type_alias.h"
#include "sema/interface/module.h"

bool sema_module_read_type_alias(SemaModule *sema, AstTypeAlias *decl, bool is_generic, bool public);
