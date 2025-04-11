#pragma once

#include "llvm/llvm.h"
#include "sema/interface/body_break.h"

void llvm_emit_bb(LlvmBackend *llvm, SemaBodyBreak *bb);
