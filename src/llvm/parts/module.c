#include "llvm-c/Core.h"
#include "llvm/private.h"
#include "llvm/parts/type.h"
#include "llvm/parts/val_decl.h"
#include "llvm/parts/decls/func_decl.h"
#include "ast/private/generic.h"
#include "core/vec.h"
#include "sema/module/behaviour/table/api.h"
#include "sema/module/decls/impl.h"
#include "sema/type/private.h"
#include "sema/value/api.h"
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
            AstGeneric *generics = node->func_decl.generics;
            if (generics) {
                SemaBehaviourTable **tables = generics->sema.tables;
                for (size_t i = 0; i < vec_len(tables); i++) {
                    SemaBehaviourTable *table = tables[i];
                    
                    // TODO: what?
                    AstGenericParam *param = &generics->params[0];
                    param->sema.type->generic.replace = table->of;
                    param->sema.type->generic.table = table;
                    llvm_emit_func_decl(llvm, table->llvm.value, &node->func_decl);
                }
            } else {
                llvm_emit_func_decl(llvm, node->func_decl.info.decl->llvm.value, &node->func_decl);
            }
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
			case AST_MODULE_NODE_EXTERNAL_FUNC: {
				node->ext_func_decl.info.decl->llvm.value = LLVMAddFunction(
					llvm_current_module(llvm),
					slice_to_cstr(&node->ext_func_decl.info.public_name),
					llvm_sema_function_type(&sema_value_typeof(node->ext_func_decl.info.decl->value)->func));
                break;
            }
			case AST_MODULE_NODE_FUNC: {
                AstGeneric *generics = node->func_decl.generics;
                if (generics) {
                    SemaBehaviourTable **tables = generics->sema.tables;
                    for (size_t i = 0; i < vec_len(tables); i++) {
                        SemaBehaviourTable *table = tables[i];
                        
                        // TODO: what???
                        AstGenericParam *param = &generics->params[0];
                        param->sema.type->generic.replace = table->of;
                        table->llvm.value = LLVMAddFunction(
                            llvm_current_module(llvm),
                            slice_to_cstr(&node->func_decl.info.name),
                            llvm_sema_function_type(&sema_value_typeof(node->func_decl.info.decl->value)->func));   
                    }
                } else {
                    node->func_decl.info.decl->llvm.value = LLVMAddFunction(
                        llvm_current_module(llvm),
                        slice_to_cstr(&node->func_decl.info.name),
                        llvm_sema_function_type(&sema_value_typeof(node->func_decl.info.decl->value)->func));
                    }
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
