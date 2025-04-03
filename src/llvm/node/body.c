#include "body.h"
#include "core/vec.h"
#include "ast/shared/body.h"
#include "llvm/node/node.h"

void llvm_emit_body(LlvmBackend *llvm, AstBody *body) {
    for (size_t i = 0; i < vec_len(body->nodes); i++) {
        AstNode *node = body->nodes[i];
        llvm_setup_node(llvm, node);
        llvm_emit_node(llvm, node);
    }
}
