#include "llvm.h"
#include "core/log.h"
#include "core/vec.h"
#include "sema/module.h"
#include "sema/project.h"
#include <llvm-c/Analysis.h>
#include <llvm-c/Core.h>
#include <llvm-c/Target.h>
#include <stdlib.h>
#include "llvm/llvm.h"
#include "llvm/node/node.h"
#include "ast/module.h"

LlvmBackend *llvm_new_host() {
    LlvmBackend *llvm = malloc(sizeof(LlvmBackend));
    LLVMInitializeNativeAsmParser();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeTarget();
    LLVMTargetRef target = LLVMGetFirstTarget();
    if (!target) {
        hob_log(LOGE, "failed to initialize target");
        return NULL;
    }
    const char *features = LLVMGetHostCPUFeatures();
    const char *cpu = LLVMGetHostCPUName();
    const char *triple = LLVMGetDefaultTargetTriple();
    llvm->machine = LLVMCreateTargetMachine(target, triple, cpu, features, LLVMCodeGenLevelNone, LLVMRelocDefault, LLVMCodeModelDefault);
    llvm->state = llvm_state(NULL, NULL, NULL);
    llvm->builder = LLVMCreateBuilder();
    llvm->module = LLVMModuleCreateWithName("main");
    return llvm;
}

void llvm_setup_module(LlvmBackend *llvm, SemaModule *module) {
    for (size_t i = 0; i < vec_len(module->ast->nodes); i++) {
        llvm_setup_node(llvm, module->ast->nodes[i]); 
    }
}

void llvm_emit_module(LlvmBackend *llvm, SemaModule *module) {
    for (size_t i = 0; i < vec_len(module->ast->nodes); i++) {
        llvm_emit_node(llvm, module->ast->nodes[i]); 
    }
}

void llvm_emit_project(LlvmBackend *llvm, SemaProject *project) {
    for (size_t i = 0; i < vec_len(project->modules); i++) {
        llvm_setup_module(llvm, project->modules[i].module);
    }
    for (size_t i = 0; i < vec_len(project->modules); i++) {
        llvm_emit_module(llvm, project->modules[i].module);
    }
}

bool llvm_verify(LlvmBackend *llvm) {
    return !LLVMVerifyModule(llvm->module, LLVMAbortProcessAction | LLVMPrintMessageAction | LLVMReturnStatusAction, NULL);
}

bool llvm_write_obj(LlvmBackend *llvm, char *path) { 
	char *error;
	if (LLVMTargetMachineEmitToFile(llvm->machine, llvm->module, path, LLVMObjectFile, &error) == 1) {
		hob_log(LOGE, "failed to emit to file: %s", error);
		return false;
	}
	return true;
}

bool llvm_write_ir(LlvmBackend *llvm, char *path) {
	char *error;	
	if (LLVMPrintModuleToFile(llvm->module, path, &error) == 1) {
		hob_log(LOGE, "failed to emit to file: %s", error);
		return false;
	}
	return true;
}
