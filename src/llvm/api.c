#include <llvm-c/Analysis.h>
#include <malloc.h>
#include "llvm/api.h"
#include "llvm/impl.h"
#include "core/log.h"

LlvmBackend *llvm_create() {
    LlvmBackend *llvm = malloc(sizeof(LlvmBackend));
	llvm->builder = LLVMCreateBuilder();
	llvm->module = LLVMModuleCreateWithName("main");
	return llvm;	
}

bool llvm_write_module_ir(LlvmBackend *llvm, char *output_path) {
	hob_log(LOGD, "emitting llvm ir dump...!");
	char *error;	
	if (LLVMPrintModuleToFile(llvm->module, output_path, &error) == 1) {
		hob_log(LOGE, "failed to emit to file: %s", error);
		return false;
	}
	return true;
}

bool llvm_write_module(LlvmBackend *llvm, char *output_path) {
	llvm_write_module_ir(llvm, "dump.ll");
	if (LLVMVerifyModule(llvm->module, LLVMAbortProcessAction | LLVMPrintMessageAction | LLVMReturnStatusAction, NULL)) {
		return false;
	}
    
	LLVMInitializeNativeTarget();
	LLVMInitializeNativeAsmPrinter();
	LLVMInitializeNativeAsmParser();

	LLVMTargetRef target = LLVMGetFirstTarget();
	// const char *target_name = "aarch64";
	//LLVMTargetRef target = LLVMGetTargetFromName(target_name);
	if (!target) {
		hob_log(LOGE, "failed to initialize target");
		return false;
	}
	const char *features = LLVMGetHostCPUFeatures();
	const char *cpu = LLVMGetHostCPUName();
	const char *triple = LLVMGetDefaultTargetTriple();
	hob_log(LOGD, "target: {cstr} - {cstr}", LLVMGetTargetName(target), LLVMGetTargetDescription(target));
	hob_log(LOGD, "triple: {cstr}", triple);
	hob_log(LOGD, "with features {cstr}", features);
	hob_log(LOGD, "cpu: {cstr}", cpu);
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
