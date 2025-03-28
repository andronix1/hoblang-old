#pragma once

#include "ast/node/decl/import.h"
#include "sema/interface/module.h"

void sema_module_read_import(SemaModule *sema, AstImport *import, bool public);
