#include "llvm.h"

bool llvm_init(LlvmBackend *llvm) {
	llvm->builder = LLVMCreateBuilder();
	llvm->module = LLVMModuleCreateWithName("main");
	return true;	
}

LLVMTypeRef llvm_sema_function_type(SemaFunction *func) {
	LLVMTypeRef *params = alloca(sizeof(LLVMTypeRef) * vec_len(func->args));
	for (size_t i = 0; i < vec_len(func->args); i++) {
		params[i] = llvm_resolve_type(func->args[i]);
	}
	return LLVMFunctionType(llvm_resolve_type(func->returning), params, vec_len(func->args), false);
}

LLVMTypeRef llvm_resolve_type(SemaType *type) {
	assert(type, "type is unresolved");
	switch (type->type) {
		case SEMA_TYPE_STRUCT: {
			LLVMTypeRef *elements = alloca(sizeof(LLVMTypeRef) * vec_len(type->struct_type->members));
			for (size_t i = 0; i < vec_len(type->struct_type->members); i++) {
				elements[i] = llvm_resolve_type(type->struct_type->members[i].type->sema);
			}
			return LLVMStructType(elements, vec_len(type->struct_type->members), false);
		}
		case SEMA_TYPE_PRIMITIVE:
			switch (type->primitive) {
				case PRIMITIVE_I8: case PRIMITIVE_U8: return LLVMInt8Type();
				case PRIMITIVE_I16: case PRIMITIVE_U16: return LLVMInt16Type();
				case PRIMITIVE_I32: case PRIMITIVE_U32: return LLVMInt32Type();
				case PRIMITIVE_I64: case PRIMITIVE_U64: return LLVMInt64Type();
				case PRIMITIVE_BOOL: return LLVMInt1Type();
				case PRIMITIVE_VOID: return LLVMVoidType();
			}
			assert(0, "invalid primitive {int}", type->primitive);
			return NULL;
		case SEMA_TYPE_FUNCTION: {
			LLVMTypeRef *params = alloca(sizeof(LLVMTypeRef) * vec_len(type->func.args));
			for (size_t i = 0; i < vec_len(type->func.args); i++) {
				params[i] = llvm_resolve_type(type->func.args[i]);
			}
			return LLVMPointerType(
				LLVMFunctionType(llvm_resolve_type(type->func.returning), params, vec_len(type->func.args), false /* IsVarArg */),
				0
			);
		}
		case SEMA_TYPE_SLICE: {
			LLVMTypeRef elements[2] = { LLVMInt64Type(), LLVMPointerType(llvm_resolve_type(type->slice_of), 0) };
			return LLVMStructType(elements, 2, false);
		}
		case SEMA_TYPE_ARRAY: {
			return LLVMArrayType(llvm_resolve_type(type->array.of), type->array.length);
		}
		case SEMA_TYPE_POINTER: {
			return LLVMPointerType(llvm_resolve_type(type->ptr_to), 0);
		}
	}
	assert(0, "invalid type {int}", type->type);
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
