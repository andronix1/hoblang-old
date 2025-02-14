#include "llvm-c/Core.h"
#include "llvm-c/Types.h"
#include "llvm/private.h"
#include "llvm/parts/type.h"
#include "llvm/parts/expr.h"
#include "llvm/parts/body.h"
#include "sema/module/decls/impl.h"
#include "sema/module/private.h"
#include "sema/type/private.h"
#include "ast/private/module_node.h"
#include "ast/private/module.h"

void llvm_module_node(LlvmBackend *llvm, AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_USE:
		case AST_MODULE_NODE_TYPE_ALIAS:
		case AST_MODULE_NODE_IMPORT:
		case AST_MODULE_NODE_EXTERNAL_FUNC:
		case AST_MODULE_NODE_STRUCT_DEF:
			break;
			
		case AST_MODULE_NODE_CONST:
			node->constant.value_decl->llvm_value = llvm_expr(llvm, node->constant.expr, true);
			break;

		case AST_MODULE_NODE_FUNC: {
			llvm_set_current_func(llvm, node->ext_func_decl.info.decl->llvm_value);
            LLVMBasicBlockRef entry = LLVMAppendBasicBlock(llvm_current_func(llvm), "entry");
            LLVMBasicBlockRef code = LLVMAppendBasicBlock(llvm_current_func(llvm), "code");

            LLVMPositionBuilderAtEnd(llvm_builder(llvm), entry);
            llvm_set_definitions_block(llvm, entry);
			for (size_t i = 0; i < vec_len(node->func_decl.info.args); i++) {
				AstFuncArg *arg = &node->func_decl.info.args[i];
				LLVMValueRef value = arg->decl->llvm_value = LLVMBuildAlloca(llvm_builder(llvm), llvm_resolve_type(arg->type.sema), slice_to_cstr(&arg->name));
				LLVMBuildStore(llvm_builder(llvm), LLVMGetParam(llvm_current_func(llvm), i), value);
			}

            LLVMPositionBuilderAtEnd(llvm_builder(llvm), code);
            llvm_set_code_block(llvm, code);
			if (llvm_body(llvm, &node->func_decl.body)) {
				llvm_body_break(llvm, &node->func_decl.body);
			}
			if (sema_type_is_primitive(node->func_decl.info.returning.sema, PRIMITIVE_VOID)) {
				LLVMBuildRetVoid(llvm_builder(llvm));
			}

            LLVMPositionBuilderAtEnd(llvm_builder(llvm), entry);
            LLVMBuildBr(llvm_builder(llvm), code);
			break;
		}
	}
}

void llvm_module_init(LlvmBackend *llvm, AstModule *module) {
	for (size_t i = 0; i < vec_len(module->nodes); i++) {
		AstModuleNode *node = &module->nodes[i];
		switch (node->type) {
			case AST_MODULE_NODE_USE:
			case AST_MODULE_NODE_TYPE_ALIAS:
			case AST_MODULE_NODE_IMPORT:
			case AST_MODULE_NODE_CONST:
			case AST_MODULE_NODE_STRUCT_DEF:
				break;
			case AST_MODULE_NODE_EXTERNAL_FUNC:
			case AST_MODULE_NODE_FUNC: {
				LLVMValueRef func = LLVMAddFunction(llvm_current_module(llvm), slice_to_cstr(&node->ext_func_decl.info.public_name), llvm_sema_function_type(&node->func_decl.info.decl->type->func));
				node->func_decl.info.decl->llvm_value = func;
				break;
            }
		}
	}
} 

void llvm_module(LlvmBackend *llvm, AstModule *module) {
	for (size_t i = 0; i < vec_len(module->nodes); i++) {
		llvm_module_node(llvm, &module->nodes[i]);
	}
}
