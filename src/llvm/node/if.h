#pragma once

#include "ast/node/stmt/if.h"
#include "llvm/interface/llvm.h"

void llvm_if(LlvmBackend *llvm, AstIf *if_else);
