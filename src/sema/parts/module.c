#include "../parts.h"

void sema_add_ast_func_info(SemaModule *sema, AstFuncInfo *info) {	
	SemaType *returning = sema_ast_type(sema, &info->returning);
	if (!returning) {
		return;
	}
	SemaType **args = vec_new(SemaType*);
	for (size_t i = 0; i < vec_len(info->args); i++) {
		AstFuncArg *arg = &info->args[i];
		SemaType *type = sema_ast_type(sema, &arg->type);
		if (!type) {
			return;
		}
		args = vec_push(args, &type);
	}
	info->decl = &sema_module_push_public_decl(sema, sema_scope_decl_new_value(
		info->name,
		sema_type_new_func(returning, args)
	))->value_decl;
}

void sema_push_ast_module_node(SemaModule *sema, AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_TYPE_ALIAS: {
			SemaType *type = sema_ast_type(sema, &node->type_alias.type);
			if (!type) {
				break;
			}
			sema_module_push_public_decl(sema, sema_scope_decl_new_type(node->type_alias.alias, type));
			break;
		}
		case AST_MODULE_NODE_USE: {
			// SemaModule *module = sema_resolve_path_module(sema, &node->use.path);
			// if (!module) {
			// 	break;
			// }
			// TODO: restore
			// sema_module_push_public_decl(sema, sema_scope_decl_new_module(
			// 	node->use.has_alias ? node->use.alias : *(Slice*)vec_top(node->use.path.segments),
			// 	module
			// ));
			break;
		}

		case AST_MODULE_NODE_IMPORT: {
			SemaModule *module = sema_project_add_module_at(sema->project, node->import.path);
			if (!module) {
				break;
			}

			sema_module_push_public_decl(sema, sema_scope_decl_new_module(node->import.as, module));
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
		arg->decl = &sema_module_push_decl(sema, sema_scope_decl_new_value(arg->name, type))->value_decl;
	}
}

void sema_ast_module_node(SemaModule *sema, AstModuleNode *node) {
	sema_module_push_scope(sema);
	switch (node->type) {
		case AST_MODULE_NODE_TYPE_ALIAS:
		case AST_MODULE_NODE_EXTERNAL_FUNC:
		case AST_MODULE_NODE_USE:
		case AST_MODULE_NODE_IMPORT:
			break;

		case AST_MODULE_NODE_FUNC:
			sema_push_ast_func_info(sema, &node->func_decl.info);
			sema->returning = node->func_decl.info.returning.sema;
			sema_ast_body(sema, &node->func_decl.body);
			break;	
	}
	sema_module_pop_scope(sema);
}

void sema_module_read(SemaModule *sema) {
	sema_module_push_scope(sema);
	sema_module_push_primitives(sema);
	for (size_t i = 0; i < vec_len(sema->ast->nodes); i++) {
		sema_push_ast_module_node(sema, &sema->ast->nodes[i]);
	}
}

void sema_module(SemaModule *sema) {
	for (size_t i = 0; i < vec_len(sema->ast->nodes); i++) {
		sema_ast_module_node(sema, &sema->ast->nodes[i]);
	}
	sema_module_pop_scope(sema);
}
