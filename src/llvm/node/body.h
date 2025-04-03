#pragma once

#include "ast/interface/body.h"
#include "llvm/interface/llvm.h"

void llvm_emit_body(LlvmBackend *llvm, AstBody *body);
