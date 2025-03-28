#include "ast/private/module_node.h"
#include "sema/module/decls/impl.h"
#include "llvm/llvm.h"
#include "llvm/parts/body.h"
#include "llvm/parts/type.h"
#include "llvm/private.h"
#include "llvm/utils/alloca.h"

void llvm_emit_func_decl(LlvmBackend *llvm, LLVMValueRef func, AstFuncDecl *func_decl) {
    llvm_set_current_func(llvm, func);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(llvm_current_func(llvm), "entry");
    LLVMBasicBlockRef code = LLVMAppendBasicBlock(llvm_current_func(llvm), "code");

    llvm_set_code_block(llvm, entry);
    LLVMPositionBuilderAtEnd(llvm_builder(llvm), entry);
    llvm_set_definitions_block(llvm, entry);
    if (func_decl->info.is_extension) {
        LLVMValueRef value = func_decl->info.self->llvm.value = llvm_alloca(llvm, llvm_resolve_type(func_decl->info.ext.of->sema));
        LLVMBuildStore(llvm_builder(llvm), LLVMGetParam(llvm_current_func(llvm), 0), value);
    }
    for (size_t i = 0; i < vec_len(func_decl->info.type.args); i++) {
        AstFuncArg *arg = &func_decl->info.type.args[i];
        LLVMValueRef value = arg->decl->llvm.value = llvm_alloca(llvm, llvm_resolve_type(arg->type->sema));
        LLVMBuildStore(llvm_builder(llvm), LLVMGetParam(llvm_current_func(llvm), i + func_decl->info.is_extension), value);
    }

    LLVMPositionBuilderAtEnd(llvm_builder(llvm), code);
    llvm_set_code_block(llvm, code);
    llvm_body(llvm, &func_decl->body);
    
    if (sema_types_equals(func_decl->info.type.returning->sema, sema_type_primitive_void())) {
        LLVMBuildRetVoid(llvm_builder(llvm));
    }

    LLVMPositionBuilderAtEnd(llvm_builder(llvm), entry);
    LLVMBuildBr(llvm_builder(llvm), code);
}
