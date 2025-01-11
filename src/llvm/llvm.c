#include "llvm.h"

bool llvm_init(LlvmBackend *llvm) {
	LLVMInitializeNativeTarget();
	LLVMInitializeNativeAsmPrinter();

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
	llvm->machine = LLVMCreateTargetMachine(target, triple, cpu, features, LLVMCodeGenLevelNone, LLVMRelocDefault, LLVMCodeModelDefault);
	if (!llvm->machine) {
		hob_log(LOGE, "failed to create target machine");
		return false;
	}
	llvm->builder = LLVMCreateBuilder();
	llvm->scopes = vec_new(LlvmScope);
	return true;	
}

LLVMTypeRef llvm_resolve_type(SemaType *type) {
	switch (type->type) {
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
			LLVMTypeRef *params = malloc(sizeof(LLVMTypeRef) * vec_len(type->func.args));
			for (size_t i = 0; i < vec_len(type->func.args); i++) {
				params[i] = llvm_resolve_type(type->func.args[i].type.sema);
			}
			return LLVMFunctionType(llvm_resolve_type(type->func.returning), params, vec_len(type->func.args), false /* IsVarArg */);
		}
	}
	assert(0, "invalid type {int}", type->type);
}

LlvmValue *llvm_resolve(LlvmBackend *llvm, Slice *name) {
	for (size_t i = 0; i < vec_len(llvm->scopes); i++) {
		for (size_t j = 0; j < vec_len(llvm->scopes[i]); j++) {
			if (slice_eq(name, llvm->scopes[i][j].name)) {
				return &llvm->scopes[i][j];
			}
		}
	}
	assert(0, "value `{slice}` wasn't registered", name);
}

LLVMTypeRef llvm_resolve_type_of(LlvmBackend *llvm, Slice *name) {
	return llvm_resolve(llvm, name)->type;
}

LLVMValueRef llvm_resolve_value(LlvmBackend *llvm, Slice *name) {
	return llvm_resolve(llvm, name)->value;
}

bool llvm_write_module(LlvmBackend *llvm, char *output_path) {
	// LLVMDumpModule(llvm->module);
	char *error;
	if (LLVMTargetMachineEmitToFile(llvm->machine, llvm->module, output_path, LLVMObjectFile, &error) == 1) {
		hob_log(LOGE, "failed to emit to file: %s", error);
		return false;
	}
	return true;
}

void llvm_push_value(LlvmBackend *llvm, LlvmValue *value) {
	LlvmScope *scope = vec_top(llvm->scopes);
	*scope = vec_push(*scope, value);
}

void llvm_pop_scope(LlvmBackend *llvm) {
	vec_pop(llvm->scopes);
}

void llvm_push_scope(LlvmBackend *llvm) {
	LlvmScope scope = vec_new(LlvmValue);
	llvm->scopes = vec_push(llvm->scopes, &scope);
}
