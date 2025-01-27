#include "../parts.h"

void llvm_module_node(LlvmBackend *llvm, AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_USE:
		case AST_MODULE_NODE_TYPE_ALIAS:
		case AST_MODULE_NODE_IMPORT:
			break;

		case AST_MODULE_NODE_EXTERNAL_FUNC: {
			LLVMValueRef func = LLVMAddFunction(llvm->module, "", llvm_sema_function_type(&node->func_decl.info.decl->type->func));
			LLVMSetInitializer(
				node->ext_func_decl.info.decl->llvm_value,
				func
			);
			LLVMSetValueName(func, slice_to_cstr(&node->ext_func_decl.info.public_name));
			break;
		}

		case AST_MODULE_NODE_FUNC: {
			llvm->func = LLVMAddFunction(llvm->module, "", llvm_sema_function_type(&node->func_decl.info.decl->type->func));
			LLVMSetInitializer(
				node->ext_func_decl.info.decl->llvm_value,
				llvm->func
			);
			LLVMSetValueName(llvm->func, slice_to_cstr(&node->func_decl.info.public_name));
			LLVMPositionBuilderAtEnd(llvm->builder, LLVMAppendBasicBlock(llvm->func, "entry"));
			for (size_t i = 0; i < vec_len(node->func_decl.info.args); i++) {
				AstFuncArg *arg = &node->func_decl.info.args[i];
				LLVMValueRef value = arg->decl->llvm_value = LLVMBuildAlloca(llvm->builder, llvm_resolve_type(arg->type.sema), "");
				LLVMBuildStore(llvm->builder, LLVMGetParam(llvm->func, i), value);
			}
			if (llvm_body(llvm, &node->func_decl.body)) {
				llvm_body_break(llvm, &node->func_decl.body);
			}
			if (sema_types_equals(node->func_decl.info.returning.sema, &primitives[PRIMITIVE_VOID])) {
				LLVMBuildRetVoid(llvm->builder);
			}
			break;
		}
	}
}

void llvm_module_init(LlvmBackend *llvm, SemaModule *module) {
	for (size_t i = 0; i < vec_len(module->public_decls); i++) {
		SemaScopeDecl *decl = module->public_decls[i];
		switch (decl->type) {
			case SEMA_SCOPE_DECL_VALUE:
				decl->value_decl.llvm_value = LLVMAddGlobal(
					llvm->module,
					llvm_resolve_type(decl->value_decl.type),
					""
				);
				break;

			case SEMA_SCOPE_DECL_MODULE:
			case SEMA_SCOPE_DECL_TYPE:
				break;
		}
	}
} 

void llvm_module(LlvmBackend *llvm, AstModule *module) {
	for (size_t i = 0; i < vec_len(module->nodes); i++) {
		llvm_module_node(llvm, &module->nodes[i]);
	}
}
