#include "val.h"
#include "core/assert.h"
#include <llvm-c/Core.h>
#include "llvm/llvm.h"
#include "llvm/node/expr.h"
#include "llvm/type.h"
#include "sema/interface/value.h"
#include "ast/shared/val_info.h"

void llvm_setup_val_info(LlvmBackend *llvm, AstValInfo *val_info, const char *name) {
    SemaConst *constant = sema_value_is_const(val_info->sema.decl->value);
    if (constant) {
        val_info->sema.decl->llvm.value = llvm_const(llvm, constant);
        return;
    }
    if (val_info->sema.is_global) {
        SemaDecl *decl = val_info->sema.decl;
        decl->llvm.value = LLVMAddGlobal(
            llvm->module,
            llvm_type(sema_value_is_runtime(decl->value)),
            ""
        );
    } else {
        assert(0, "NIY");
    }
}

void llvm_setup_val_decl(LlvmBackend *llvm, AstValDecl *val_decl, const char *name) {
    llvm_setup_val_info(llvm, val_decl->info, name);
}

void llvm_emit_val_decl(LlvmBackend *llvm, AstValDecl *decl) {
    if (sema_value_is_const(decl->info->sema.decl->value)) {
        return;
    }
    assert(0, "NIY");
}

