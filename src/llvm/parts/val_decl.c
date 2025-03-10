#include "llvm/parts/val_decl.h"
#include "ast/private/val_decl.h"
#include "sema/module/decls/impl.h"
#include "sema/value/api.h"
#include "sema/value/value.h"
#include "llvm/parts/expr.h"
#include "llvm/parts/type.h"
#include "llvm/private.h"
#include "llvm/utils/alloca.h"
#include <llvm-c/Core.h>

void llvm_val_decl_global_set(LlvmBackend *llvm, AstValDecl *val_decl) {
    if (!val_decl->initializer) {
        return;
    }
    SemaValue *sema_value = val_decl->sema.decl->value;
    switch (val_decl->type) {
        case AST_VAL_DECL_CONST:
            break;
        case AST_VAL_DECL_FINAL:
            val_decl->sema.decl->llvm.value = llvm_expr(llvm, val_decl->initializer, true);
            break;
        case AST_VAL_DECL_VAR: 
            LLVMSetInitializer(
                val_decl->sema.decl->llvm.value,
                llvm_expr(llvm, val_decl->initializer, true)
            );
            break;
    }
}

void llvm_val_decl_global_init(LlvmBackend *llvm, AstValDecl *val_decl) {
    SemaValue *sema_value = val_decl->sema.decl->value;
    switch (val_decl->type) {
        case AST_VAL_DECL_FINAL:
            break;
        case AST_VAL_DECL_CONST:
            val_decl->sema.decl->llvm.value = llvm_expr(llvm, val_decl->initializer, true);
            break;
        case AST_VAL_DECL_VAR: 
            val_decl->sema.decl->llvm.value = LLVMAddGlobal(
                llvm_current_module(llvm),
                llvm_resolve_type(sema_value_typeof(sema_value)),
                ""
            );
            break;
    }
}

void llvm_val_decl(LlvmBackend *llvm, AstValDecl *val_decl) {
    SemaValue *sema_value = val_decl->sema.decl->value;
    if (sema_value_is_const(sema_value)) {
        return;
    }
    if (sema_value_is_final(sema_value)) {
        val_decl->sema.decl->llvm.value = llvm_expr(llvm, val_decl->initializer, true);
        return;
    }
    LLVMValueRef value = val_decl->sema.decl->llvm.value = llvm_alloca(
        llvm,
        llvm_resolve_type(sema_value_typeof(sema_value))
    );
    if (val_decl->initializer) {
        LLVMBuildStore(
            llvm_builder(llvm),
            llvm_expr(llvm, val_decl->initializer, true),
            value
        );
    }
}
