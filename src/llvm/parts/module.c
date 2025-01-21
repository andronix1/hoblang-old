#include "../parts.h"

void llvm_module_node(LlvmBackend *llvm, AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_USE:
		case AST_MODULE_NODE_TYPE_ALIAS:
		case AST_MODULE_NODE_IMPORT:
			break;

		case AST_MODULE_NODE_EXTERNAL_FUNC:
			LLVMSetValueName(llvm_sema_value(llvm, node->ext_func_decl.info.decl), slice_to_cstr(&node->ext_func_decl.info.public_name));
			break;

		case AST_MODULE_NODE_FUNC: {
			llvm->func = llvm_sema_value(llvm, node->func_decl.info.decl);
			LLVMSetValueName(llvm->func, slice_to_cstr(&node->func_decl.info.public_name));
			LLVMPositionBuilderAtEnd(llvm->builder, LLVMAppendBasicBlock(llvm->func, "entry"));
			for (size_t i = 0; i < vec_len(node->func_decl.info.args); i++) {
				AstFuncArg *arg = &node->func_decl.info.args[i];
				LLVMValueRef value = arg->decl->llvm_value = LLVMBuildAlloca(llvm->builder, llvm_resolve_type(arg->type.sema), "");
				LLVMBuildStore(llvm->builder, LLVMGetParam(llvm->func, i), value);
			}
			llvm_body(llvm, &node->func_decl.body);
			llvm_body_break(llvm, &node->func_decl.body);
			if (sema_types_equals(node->func_decl.info.returning.sema, &primitives[PRIMITIVE_VOID])) {
				LLVMBuildRetVoid(llvm->builder);
			}
			break;
		}
	}
}

void llvm_module(LlvmBackend *llvm, AstModule *module) {
	for (size_t i = 0; i < vec_len(module->nodes); i++) {
		llvm_module_node(llvm, &module->nodes[i]);
	}
}
