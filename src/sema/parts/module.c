#include "../parts.h"

void sema_add_ast_func_info(Sema *sema, AstFuncInfo *info) {	
	SemaRecord decl = {
		.name = info->name,
	};
	SemaType *returning = sema_ast_type(sema, &info->returning);
	if (!returning) {
		return;
	}
	decl.type = sema_type_new_func(returning, info->args);
	for (size_t i = 0; i < vec_len(info->args); i++) {
		AstFuncArg *arg = &info->args[i];
		if (!sema_ast_type(sema, &arg->type)) {
			return;
		}
	}
	sema_push_decl(sema, &decl);
}

void sema_push_ast_module_node(Sema *sema, AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_FUNC:
			sema_add_ast_func_info(sema, &node->func_decl.info);
			break;
		
		case AST_MODULE_NODE_EXTERNAL_FUNC:
			sema_add_ast_func_info(sema, &node->ext_func_decl.info);
			break;
	}
}

void sema_push_ast_func_info(Sema *sema, AstFuncInfo *info) {
	for (size_t i = 0; i < vec_len(info->args); i++) {
		AstFuncArg *arg = &info->args[i];
		SemaRecord record = {
			.name = arg->name
		};
		if (!(record.type = sema_ast_type(sema, &arg->type))) {
			continue;
		}
		sema_push_decl(sema, &record);
	}
}

void sema_ast_module_node(Sema *sema, AstModuleNode *node) {
	sema_push_scope(sema);
	switch (node->type) {
		case AST_MODULE_NODE_FUNC:
			sema_push_ast_func_info(sema, &node->func_decl.info);
			sema->returning = node->func_decl.info.returning.sema;
			sema_ast_body(sema, &node->func_decl.body);
			break;
		
		case AST_MODULE_NODE_EXTERNAL_FUNC:
			sema_push_ast_func_info(sema, &node->func_decl.info);
			break;
	}
	sema_pop_scope(sema);
}

void sema_ast_module(Sema *sema, AstModule *module) {
	sema_push_scope(sema);
	sema_push_primitives(sema);
	for (size_t i = 0; i < vec_len(module->nodes); i++) {
		sema_push_ast_module_node(sema, &module->nodes[i]);
	}
	for (size_t i = 0; i < vec_len(module->nodes); i++) {
		sema_ast_module_node(sema, &module->nodes[i]);
	}
	sema_pop_scope(sema);
}
