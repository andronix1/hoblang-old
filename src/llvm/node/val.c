#include "val.h"
#include "core/assert.h"
#include <llvm-c/Core.h>
#include "llvm/alloca.h"
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
    SemaDecl *decl = val_info->sema.decl;
    LLVMTypeRef type = llvm_type(sema_value_is_runtime(decl->value));
    bool is_var = sema_value_is_var(decl->value);
    if (val_info->sema.is_global) {
        assert(is_var, "only vars can be initialized as global");
        decl->llvm.value = LLVMAddGlobal(llvm->module, type, "");
    } else {
        if (is_var) {
            decl->llvm.value = llvm_alloca(llvm, type, NULL);
        }
    }
}

void llvm_setup_val_decl(LlvmBackend *llvm, AstValDecl *val_decl, const char *name) {
    llvm_setup_val_info(llvm, val_decl->info, name);
}

void llvm_emit_val_decl(LlvmBackend *llvm, AstValDecl *decl) {
    if (sema_value_is_const(decl->info->sema.decl->value)) {
        return;
    }
    bool is_var = sema_value_is_var(decl->info->sema.decl->value);
    if (decl->initializer) {
        if (is_var) {
            LLVMBuildStore(
                llvm->builder,
                llvm_expr_get(llvm, decl->initializer),
                decl->info->sema.decl->llvm.value
            );
        } else {
            decl->info->sema.decl->llvm.value = llvm_expr_get(llvm, decl->initializer);
        }
    }
}

