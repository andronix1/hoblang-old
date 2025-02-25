#include <llvm-c/Analysis.h>
#include <malloc.h>
#include "llvm/api.h"
#include "llvm/impl.h"
#include "llvm/llvm.h"
#include "core/log.h"
#include "sema/module/impl.h"
#include "sema/module/api.h"
#include "core/vec.h"
#include "sema/project/project.h"
#include "sema/project/api.h"

LlvmBackend *llvm_create() {
    LlvmBackend *llvm = malloc(sizeof(LlvmBackend));
	llvm->builder = LLVMCreateBuilder();
	llvm->module = LLVMModuleCreateWithName("main");
	return llvm;	
}

bool llvm_emit_project(LlvmBackend *llvm, SemaProject *project) {
    SemaProjectModule **modules = sema_project_modules(project);
    for (size_t i = 0; i < vec_len(modules); i++) {
        llvm_module_init(llvm, sema_module_of(sema_project_module_inner(modules[i])));
    }
    for (size_t i = 0; i < vec_len(modules); i++) {
        llvm_module(llvm, sema_module_of(sema_project_module_inner(modules[i])));
    }
    return true;
}

bool llvm_write_module_ir(LlvmBackend *llvm, char *output_path) {
	char *error;	
	if (LLVMPrintModuleToFile(llvm->module, output_path, &error) == 1) {
		hob_log(LOGE, "failed to emit to file: %s", error);
		return false;
	}
	return true;
}

bool llvm_write_module(LlvmBackend *llvm, char *output_path) {
	if (LLVMVerifyModule(llvm->module, LLVMAbortProcessAction | LLVMPrintMessageAction | LLVMReturnStatusAction, NULL)) {
		return false;
	}
    
	LLVMInitializeNativeTarget();
	LLVMInitializeNativeAsmPrinter();
	LLVMInitializeNativeAsmParser();

	LLVMTargetRef target = LLVMGetFirstTarget();
	if (!target) {
		hob_log(LOGE, "failed to initialize target");
		return false;
	}
	const char *features = LLVMGetHostCPUFeatures();
	const char *cpu = LLVMGetHostCPUName();
	const char *triple = LLVMGetDefaultTargetTriple();
	// hob_log(LOGD, "target: {cstr} - {cstr}", LLVMGetTargetName(target), LLVMGetTargetDescription(target));
	// hob_log(LOGD, "triple: {cstr}", triple);
	// hob_log(LOGD, "with features {cstr}", features);
	// hob_log(LOGD, "cpu: {cstr}", cpu);
	llvm->machine = LLVMCreateTargetMachine(target, triple, cpu, features, LLVMCodeGenLevelAggressive, LLVMRelocDefault, LLVMCodeModelDefault);
	if (!llvm->machine) {
		hob_log(LOGE, "failed to create target machine");
		return false;
	}
	char *error;
	if (LLVMTargetMachineEmitToFile(llvm->machine, llvm->module, output_path, LLVMObjectFile, &error) == 1) {
		hob_log(LOGE, "failed to emit to file: %s", error);
		return false;
	}
	return true;
}
