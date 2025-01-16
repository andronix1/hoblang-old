#include "../parts.h"

LLVMValueRef llvm_resolve_func(LlvmBackend *llvm, AstFuncInfo *func) {
	if (func->decl->llvm_value) {
		return func->decl->llvm_value;
	}
	LLVMTypeRef *params = malloc(sizeof(LLVMTypeRef) * vec_len(func->args));
	for (size_t i = 0; i < vec_len(func->args); i++) {
		params[i] = llvm_resolve_type(func->args[i].type.sema);
	}
	LLVMTypeRef type = LLVMFunctionType(llvm_resolve_type(func->returning.sema), params, vec_len(func->args), false);
	return func->decl->llvm_value = LLVMAddFunction(llvm->module, slice_to_cstr(&func->public_name), type);
}

void llvm_add_module_node(LlvmBackend *llvm, AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_USE: case AST_MODULE_NODE_IMPORT: break;
		case AST_MODULE_NODE_EXTERNAL_FUNC: llvm_resolve_func(llvm, &node->ext_func_decl.info); break;
		case AST_MODULE_NODE_FUNC: llvm_resolve_func(llvm, &node->func_decl.info); break;
	}
}

void llvm_module_node(LlvmBackend *llvm, AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_USE:
		case AST_MODULE_NODE_IMPORT:
		case AST_MODULE_NODE_EXTERNAL_FUNC:
			break;

		case AST_MODULE_NODE_FUNC: {
			llvm->func = llvm_resolve_func(llvm, &node->func_decl.info);
			LLVMPositionBuilderAtEnd(llvm->builder, LLVMAppendBasicBlock(llvm->func, "entry"));
			for (size_t i = 0; i < vec_len(node->func_decl.info.args); i++) {
				AstFuncArg *arg = &node->func_decl.info.args[i];
				LLVMValueRef value = arg->decl->llvm_value = LLVMBuildAlloca(llvm->builder, llvm_resolve_type(arg->type.sema), "");
				LLVMBuildStore(llvm->builder, LLVMGetParam(llvm->func, i), value);
			}
			llvm_body(llvm, &node->func_decl.body);
			if (sema_types_equals(node->func_decl.info.returning.sema, &primitives[PRIMITIVE_VOID])) {
				LLVMBuildRetVoid(llvm->builder);
			}
			break;
		}
	}
}

void llvm_module(LlvmBackend *llvm, AstModule *module) {
	llvm->module = LLVMModuleCreateWithName(slice_to_cstr(&module->name));
	for (size_t i = 0; i < vec_len(module->nodes); i++) {
		llvm_add_module_node(llvm, &module->nodes[i]);
	}
	for (size_t i = 0; i < vec_len(module->nodes); i++) {
		llvm_module_node(llvm, &module->nodes[i]);
	}
}
