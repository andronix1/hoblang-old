#include <llvm-c/Analysis.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <malloc.h>
#include <stdio.h>
#include "llvm/api.h"
#include "llvm/impl.h"
#include "llvm/llvm.h"
#include "core/log.h"
#include "sema/module/api.h"
#include "core/vec.h"
#include "core/ansi.h"
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

void llvm_print_targets() {
    LLVMInitializeAllTargetInfos();
    for (
        LLVMTargetRef target = LLVMGetFirstTarget();
        target;
        target = LLVMGetNextTarget(target)
    ) {
        printf(ANSI_BOLD "%s" ANSI_RESET " — %s\n",
            LLVMGetTargetName(target),
            LLVMGetTargetDescription(target)
        );
    }
}

bool llvm_write_module_ir(LlvmBackend *llvm, char *output_path) {
	char *error;	
	if (LLVMPrintModuleToFile(llvm->module, output_path, &error) == 1) {
		hob_log(LOGE, "failed to emit to file: %s", error);
		return false;
	}
	return true;
}

bool llvm_write_module(LlvmBackend *llvm, bool verify, const char *target_name, char *output_path) {
	if (verify && LLVMVerifyModule(llvm->module, LLVMAbortProcessAction | LLVMPrintMessageAction | LLVMReturnStatusAction, NULL)) {
        printf("failed to verify module! continue? [y/N] ");
        if (getchar() != 'y') {
            return false;    
        }
	}
    
	LLVMTargetRef target = NULL;
    if (!target_name) {
        LLVMInitializeNativeTarget();
        LLVMInitializeNativeAsmPrinter();
        LLVMInitializeNativeAsmParser();
        target = LLVMGetFirstTarget();
        if (!target) {
            hob_log(LOGE, "failed to initialize target");
            return false;
        }
        const char *features = LLVMGetHostCPUFeatures();
        const char *cpu = LLVMGetHostCPUName();
        const char *triple = LLVMGetDefaultTargetTriple();
        llvm->machine = LLVMCreateTargetMachine(target, triple, cpu, features, LLVMCodeGenLevelAggressive, LLVMRelocDefault, LLVMCodeModelDefault);
    } else {
        LLVMInitializeAllTargets();
        LLVMInitializeAllTargetInfos();
        LLVMInitializeAllTargetMCs();
        LLVMInitializeAllAsmPrinters();
        LLVMInitializeAllAsmParsers();
        target = LLVMGetTargetFromName(target_name);
        if (!target) {
            hob_log(LOGE, "failed to initialize target `{cstr}`", target_name);
            return false;
        }
        llvm->machine = LLVMCreateTargetMachine(target, target_name, "", "", LLVMCodeGenLevelAggressive, LLVMRelocDefault, LLVMCodeModelDefault);
    }
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
