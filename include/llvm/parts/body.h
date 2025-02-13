#pragma once

#include <stdbool.h>
#include "llvm/llvm.h"
#include "ast/api/body.h"

void llvm_body_break(LlvmBackend *llvm, AstBody *body);
bool llvm_body(LlvmBackend *llvm, AstBody *body);
