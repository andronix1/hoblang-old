#include "../parts.h"

void sema_add_ast_func_info(SemaModule *sema, AstFuncInfo *info) {	
	SemaType *returning = sema_ast_type(sema, &info->returning);
	if (!returning) {
		return;
	}
	for (size_t i = 0; i < vec_len(info->args); i++) {
		AstFuncArg *arg = &info->args[i];
		if (!sema_ast_type(sema, &arg->type)) {
			return;
		}
	}
	info->decl = sema_push_decl(sema, info->name, sema_type_new_func(returning, info->args));
}

void sema_push_ast_module_node(SemaModule *sema, AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_USE: {
			SemaModuleUsage usage = {
				.module = sema_resolve_module_path(sema, &node->use.path),
				.name = node->use.has_alias ? node->use.alias : *(Slice*)vec_top(node->use.path.segments)
			};
			if (usage.module) {
				sema->modules = vec_push(sema->modules, &usage);
			}
			break;
		}

		case AST_MODULE_NODE_IMPORT: {
			SemaModuleUsage usage = {
				.module = sema_project_add_module(sema->project, node->import.path),
				.name = node->import.as
			};
			if (usage.module) {
				sema->modules = vec_push(sema->modules, &usage);
			}
			break;
		}

		case AST_MODULE_NODE_FUNC:
			sema_add_ast_func_info(sema, &node->func_decl.info);
			break;
		
		case AST_MODULE_NODE_EXTERNAL_FUNC:
			sema_add_ast_func_info(sema, &node->ext_func_decl.info);
			break;
	}
}

void sema_push_ast_func_info(SemaModule *sema, AstFuncInfo *info) {
	for (size_t i = 0; i < vec_len(info->args); i++) {
		AstFuncArg *arg = &info->args[i];
		SemaType *type = sema_ast_type(sema, &arg->type);
		arg->decl = sema_push_decl(sema, arg->name, type);
	}
}

void sema_ast_module_node(SemaModule *sema, AstModuleNode *node) {
	sema_push_scope(sema);
	switch (node->type) {
		case AST_MODULE_NODE_EXTERNAL_FUNC: case AST_MODULE_NODE_USE: case AST_MODULE_NODE_IMPORT: break;
		case AST_MODULE_NODE_FUNC:
			sema_push_ast_func_info(sema, &node->func_decl.info);
			sema->returning = node->func_decl.info.returning.sema;
			sema_ast_body(sema, &node->func_decl.body);
			break;	
	}
	sema_pop_scope(sema);
}

void sema_module_read(SemaModule *sema) {
	sema_push_scope(sema);
	sema_push_primitives(sema);
	for (size_t i = 0; i < vec_len(sema->ast->nodes); i++) {
		sema_push_ast_module_node(sema, &sema->ast->nodes[i]);
	}
}

void sema_module(SemaModule *sema) {
	for (size_t i = 0; i < vec_len(sema->ast->nodes); i++) {
		sema_ast_module_node(sema, &sema->ast->nodes[i]);
	}
}
