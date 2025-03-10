#include "ast/private/body.h"
#include "ast/private/func_info.h"
#include "core/vec.h"
#include "sema/module/decls/impl.h"
#include "sema/type/private.h"
#include "sema/value/api.h"
#include "llvm/llvm.h"
#include "llvm/parts/body.h"
#include "llvm/parts/type.h"
#include "llvm/private.h"
#include "llvm/utils/alloca.h"
#include <llvm-c/Core.h>
#include <llvm-c/Types.h>

void llvm_emit_func(LlvmBackend *llvm, LLVMValueRef func, SemaDecl *ext, AstFuncTypeInfo *info, AstBody *body) {
    AstFuncArg *args = info->args;
    SemaType *returning = info->returning->sema;
    LLVMValueRef curr_func = llvm_current_func(llvm);
    LLVMBasicBlockRef code_block = llvm_code_block(llvm);
    LLVMBasicBlockRef def_block = llvm_definitions_block(llvm);

    llvm_set_current_func(llvm, func);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(llvm_current_func(llvm), "entry");
    LLVMBasicBlockRef code = LLVMAppendBasicBlock(llvm_current_func(llvm), "code");

    LLVMPositionBuilderAtEnd(llvm_builder(llvm), entry);
    llvm_set_definitions_block(llvm, entry);
    if (ext) {
        LLVMValueRef value = ext->llvm.value = llvm_alloca(llvm, llvm_resolve_type(sema_value_typeof(ext->value)));
        LLVMBuildStore(llvm_builder(llvm), LLVMGetParam(llvm_current_func(llvm), 0), value);
    }
    for (size_t i = 0; i < vec_len(args); i++) {
        AstFuncArg *arg = &args[i];
        LLVMValueRef value = arg->decl->llvm.value = llvm_alloca(llvm, llvm_resolve_type(arg->type->sema));
        LLVMPositionBuilderAtEnd(llvm_builder(llvm), entry);
        LLVMBuildStore(llvm_builder(llvm), LLVMGetParam(llvm_current_func(llvm), i + (ext ? 1 : 0)), value);
    }

    LLVMPositionBuilderAtEnd(llvm_builder(llvm), code);
    llvm_set_code_block(llvm, code);
    llvm_body(llvm, body);
    
    if (sema_types_equals(returning, sema_type_primitive_void())) {
        LLVMBuildRetVoid(llvm_builder(llvm));
    }

    LLVMPositionBuilderAtEnd(llvm_builder(llvm), entry);
    LLVMBuildBr(llvm_builder(llvm), code);

    llvm_set_current_func(llvm, curr_func);
    llvm_set_code_block(llvm, code_block);
    llvm_set_definitions_block(llvm, def_block);
    LLVMPositionBuilderAtEnd(llvm_builder(llvm), code_block);
}
