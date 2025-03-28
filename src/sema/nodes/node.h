#pragma once

#include "ast/interface/node.h"
#include "sema/interface/module.h"

void sema_module_analyze_node(SemaModule *sema, AstNode *node);
void sema_module_read_node(SemaModule *sema, AstNode *node);
