#include "llvm/private.h"
#include <llvm-c/Core.h>
#include <llvm-c/Types.h>
#include "sema/module/loop/private.h"
#include "ast/private/stmts/loop_control.h"

void llvm_stmt_continue(LlvmBackend *llvm, AstStmtLoopControl *loop_control) {
    LLVMBuildBr(llvm_builder(llvm), loop_control->loop->llvm.continue_block);
}
