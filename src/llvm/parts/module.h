#pragma once

#include "llvm/llvm.h"
#include "ast/api/module.h"

void llvm_module(LlvmBackend *llvm, AstModule *module);
