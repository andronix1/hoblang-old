#pragma once

#include "ast/interface/body.h"
#include "sema/interface/module.h"

typedef struct SemaBodyBreak SemaBodyBreak;

SemaBodyBreak *sema_module_bb_collect_until(SemaModule *sema, AstBody *until);
