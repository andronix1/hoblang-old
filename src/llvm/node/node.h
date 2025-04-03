#pragma once

#include "ast/interface/node.h"
#include "llvm/llvm.h"

void llvm_emit_node(LlvmBackend *llvm, AstNode *node);
void llvm_setup_node(LlvmBackend *llvm, AstNode *node);

