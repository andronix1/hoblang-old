#include "body_break.h"
#include "sema/body_break.h"
#include "llvm/node/body.h"

void llvm_emit_bb(LlvmBackend *llvm, SemaBodyBreak *bb) {
    for (size_t i = 0; i < vec_len(bb->defers); i++) {
        llvm_emit_body(llvm, bb->defers[i]);
    }
}
