#pragma once

#include "ast/interface/generic.h"
#include "sema/interface/module.h"

void sema_module_push_generics(SemaModule *module, AstGenerics *generics);
