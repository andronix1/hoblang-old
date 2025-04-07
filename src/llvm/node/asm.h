#pragma once

#include "ast/node/stmt/asm.h"
#include "llvm/interface/llvm.h"

void llvm_inline_asm(LlvmBackend *llvm, AstInlineAsm *inline_asm);
