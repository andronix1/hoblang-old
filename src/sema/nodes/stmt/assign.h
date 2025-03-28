#pragma once

#include "sema/interface/module.h"
#include "ast/node/stmt/assign.h"

void sema_analyze_stmt_assign(SemaModule *sema, AstAssign *assign);

