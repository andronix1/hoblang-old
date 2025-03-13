#include "llvm-c/Core.h"
#include "llvm-c/Types.h"
#include "llvm/private.h"
#include "llvm/parts/type.h"
#include "llvm/parts/body.h"
#include "llvm/parts/val_decl.h"
#include "llvm/utils/alloca.h"
#include "sema/module/decls/impl.h"
#include "sema/type/private.h"
#include "sema/value/private.h"
#include "ast/private/module_node.h"
#include "ast/private/module.h"

void llvm_module_node(LlvmBackend *llvm, AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_BEHAVIOUR_DECL:
		case AST_MODULE_NODE_USE:
		case AST_MODULE_NODE_FROM_USE:
		case AST_MODULE_NODE_TYPE_ALIAS:
		case AST_MODULE_NODE_IMPORT:
		case AST_MODULE_NODE_EXTERNAL_FUNC:
		case AST_MODULE_NODE_EXTERNAL_VAR:
		case AST_MODULE_NODE_STRUCT_DEF:
			break;
			
		case AST_MODULE_NODE_VAL_DECL:
            llvm_val_decl_global_set(llvm, &node->val_decl);
			break;

		case AST_MODULE_NODE_FUNC: {
			llvm_set_current_func(llvm, node->func_decl.info.decl->llvm.value);
            LLVMBasicBlockRef entry = LLVMAppendBasicBlock(llvm_current_func(llvm), "entry");
            LLVMBasicBlockRef code = LLVMAppendBasicBlock(llvm_current_func(llvm), "code");

            llvm_set_code_block(llvm, entry);
            LLVMPositionBuilderAtEnd(llvm_builder(llvm), entry);
            llvm_set_definitions_block(llvm, entry);
			if (node->func_decl.info.is_extension) {
				LLVMValueRef value = node->func_decl.info.self->llvm.value = llvm_alloca(llvm, llvm_resolve_type(node->func_decl.info.ext.of->sema));
				LLVMBuildStore(llvm_builder(llvm), LLVMGetParam(llvm_current_func(llvm), 0), value);
			}
			for (size_t i = 0; i < vec_len(node->func_decl.info.type.args); i++) {
				AstFuncArg *arg = &node->func_decl.info.type.args[i];
				LLVMValueRef value = arg->decl->llvm.value = llvm_alloca(llvm, llvm_resolve_type(arg->type->sema));
				LLVMBuildStore(llvm_builder(llvm), LLVMGetParam(llvm_current_func(llvm), i + node->func_decl.info.is_extension), value);
			}

            LLVMPositionBuilderAtEnd(llvm_builder(llvm), code);
            llvm_set_code_block(llvm, code);
			llvm_body(llvm, &node->func_decl.body);
            
            if (sema_types_equals(node->func_decl.info.type.returning->sema, sema_type_primitive_void())) {
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
			case AST_MODULE_NODE_VAL_DECL:
                llvm_val_decl_global_init(llvm, &node->val_decl);
                break;
		    case AST_MODULE_NODE_BEHAVIOUR_DECL:
		    case AST_MODULE_NODE_FROM_USE:
			case AST_MODULE_NODE_USE:
			case AST_MODULE_NODE_TYPE_ALIAS:
			case AST_MODULE_NODE_IMPORT:
			case AST_MODULE_NODE_STRUCT_DEF:
				break;
			case AST_MODULE_NODE_EXTERNAL_VAR: {
				node->ext_var_decl.sema.decl->llvm.value = LLVMAddGlobal(
					llvm_current_module(llvm),
                    llvm_resolve_type(node->ext_var_decl.type->sema),
					slice_to_cstr(&node->ext_var_decl.public_name)
                );
                break;
            }
			case AST_MODULE_NODE_EXTERNAL_FUNC:
			case AST_MODULE_NODE_FUNC: {
				LLVMValueRef func = LLVMAddFunction(
					llvm_current_module(llvm),
					slice_to_cstr(&node->ext_func_decl.info.public_name),
					llvm_sema_function_type(&node->func_decl.info.decl->value->sema_type->func));
				node->func_decl.info.decl->llvm.value = func;
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
