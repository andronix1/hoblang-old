#pragma once

#include "sema/interface/module.h"
#include "ast/node/stmt/asm.h"

void sema_analyze_inline_asm(SemaModule *sema, AstInlineAsm *inline_asm);
