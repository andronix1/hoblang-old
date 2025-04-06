#include "func.h"
#include "core/vec.h"
#include "llvm/alloca.h"
#include "llvm/llvm.h"
#include "llvm/node/body.h"
#include "llvm/type.h"
#include <llvm-c/Core.h>
#include "ast/shared/func_info.h"

void llvm_setup_func_info(LlvmBackend *llvm, AstFuncInfo *func_info, const char *name) {
    func_info->sema.decl->llvm.value = LLVMAddFunction(llvm->module, name,
            llvm_type(func_info->sema.type));
}

void llvm_setup_func_decl(LlvmBackend *llvm, AstFuncDecl *func_decl, const char *name) {
    llvm_setup_func_info(llvm, func_decl->info, name);
}

void llvm_emit_func_decl(LlvmBackend *llvm, AstFuncDecl *decl) {
    LLVMValueRef cur_func = decl->info->sema.decl->llvm.value;
    LLVMValueRef func = llvm_switch_func(llvm, cur_func);
    size_t args_offset = 0;
    // TODO: alloc on stack
    LLVMPositionBuilderAtEnd(llvm->builder, LLVMAppendBasicBlock(cur_func, ""));
    if (decl->info->ext_of) {
        args_offset = 1;
        decl->sema.self->llvm.value = llvm_alloca(llvm, llvm_type(sema_value_is_runtime(decl->sema.self->value)), LLVMGetParam(llvm->func, 0));
    }
    for (size_t i = 0; i < vec_len(decl->info->args); i++) {
        SemaDecl *arg_decl = decl->info->args[i].sema.decl;
        arg_decl->llvm.value = llvm_alloca(llvm, llvm_type(sema_value_is_runtime(arg_decl->value)), LLVMGetParam(llvm->func, i + args_offset));
    }
    llvm_emit_body(llvm, decl->body);
    llvm_switch_func(llvm, func);
}
