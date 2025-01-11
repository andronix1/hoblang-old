#include "../parts.h"

void llvm_add_func(LlvmBackend *llvm, AstFuncInfo *func, Slice *override_name) {
	LLVMTypeRef *params = malloc(sizeof(LLVMTypeRef) * vec_len(func->args));
	for (size_t i = 0; i < vec_len(func->args); i++) {
		params[i] = llvm_resolve_type(func->args[i].type.sema);
	}
	LLVMTypeRef type = LLVMFunctionType(llvm_resolve_type(func->returning.sema), params, vec_len(func->args), false);
	LLVMValueRef value = LLVMAddFunction(llvm->module, slice_to_cstr(override_name ? override_name : &func->name), type);
	LlvmValue val = {
		.name = &func->name,
		.type = type,
		.value = value,
		.ptr = false
	};
	llvm_push_value(llvm, &val);
}

void llvm_add_module_node(LlvmBackend *llvm, AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_FUNC: {
			llvm_add_func(llvm, &node->func_decl.info, NULL);
			break;
		}
		case AST_MODULE_NODE_EXTERNAL_FUNC: {
			llvm_add_func(llvm, &node->ext_func_decl.info, &node->ext_func_decl.ext_name);
			break;
		}
	}
}

void llvm_module_node(LlvmBackend *llvm, AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_FUNC: {
			llvm_push_scope(llvm);
			llvm->func = llvm_resolve_value(llvm, &node->func_decl.info.name);
			LLVMPositionBuilderAtEnd(llvm->builder, LLVMAppendBasicBlock(llvm->func, "entry"));
			for (size_t i = 0; i < vec_len(node->func_decl.info.args); i++) {
				AstFuncArg *arg = &node->func_decl.info.args[i];
				LlvmValue value = {
					.type = llvm_resolve_type(arg->type.sema),
					.value = LLVMGetParam(llvm->func, i),
					.name = &arg->name
				};
				llvm_push_value(llvm, &value);
			}
			llvm_body(llvm, &node->func_decl.body);
			if (sema_types_equals(node->func_decl.info.returning.sema, &primitives[PRIMITIVE_VOID])) {
				LLVMBuildRetVoid(llvm->builder);
			}
			llvm_pop_scope(llvm);
			break;
		}
		case AST_MODULE_NODE_EXTERNAL_FUNC: break;
	}
}

void llvm_module(LlvmBackend *llvm, AstModule *module) {
	llvm_push_scope(llvm);
	llvm->module = LLVMModuleCreateWithName(slice_to_cstr(&module->name));
	for (size_t i = 0; i < vec_len(module->nodes); i++) {
		llvm_add_module_node(llvm, &module->nodes[i]);
	}
	for (size_t i = 0; i < vec_len(module->nodes); i++) {
		llvm_module_node(llvm, &module->nodes[i]);
	}
	llvm_pop_scope(llvm);
}
