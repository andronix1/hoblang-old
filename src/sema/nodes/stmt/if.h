#pragma once

#include "ast/node/stmt/if.h"
#include "sema/interface/module.h"

void sema_analyze_stmt_if(SemaModule *sema, AstIf *if_else);
