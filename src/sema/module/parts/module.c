#include "sema/project/api.h"
#include "sema/module/private.h"
#include "sema/module/api.h"
#include "sema/module/decls/impl.h"
#include "sema/type/private.h"
#include "core/vec.h"
#include "ast/private/module.h"
#include "sema/module/parts/path.h"
#include "sema/module/parts/body.h"
#include "sema/module/parts/expr.h"
#include "sema/module/parts/type.h"

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
		sema_type_new_func(returning, args),
		true
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
		case AST_MODULE_NODE_STRUCT_DEF: {
			for (size_t i = 0; i < vec_len(node->struct_def.members); i++) {
				AstStructMember *member = &node->struct_def.members[i];
				for (size_t j = 0; j < i; j++) {
					AstStructMember *jmember = &node->struct_def.members[j];
					if (slice_eq(&member->name, &jmember->name)) {
						sema_err("field {slice} duplicated", &member->name);
					}
				}
				sema_ast_type(sema, member->type);
			}
			sema_module_push_public_decl(sema, sema_scope_decl_new_type(node->struct_def.name, sema_type_new_struct(&node->struct_def)));
			break;
		}
		case AST_MODULE_NODE_CONST: {
			SemaType *const_type = sema_ast_type(sema, &node->constant.type);

			if (const_type) {
				node->constant.value_decl = &sema_module_push_public_decl(sema, sema_scope_decl_new_value(
					node->constant.name,
					const_type,
					true
				))->value_decl;
				node->constant.value_decl->integer_expr = node->constant.expr;
			}
			break;
		}

		case AST_MODULE_NODE_USE: {
			SemaScopeDecl *decl = sema_resolve_decl_path_raw(sema, &node->use.path);
			if (!decl) {
				break;
			}
			sema_module_push_public_decl(sema, decl);
			// TODO: alias
			//  sema_scope_decl_new_module(
			// 	node->use.has_alias ? node->use.alias : *(Slice*)vec_top(node->use.path.segments),
			// 	module
			// ));
			break;
		}

		case AST_MODULE_NODE_IMPORT: {
			SemaModule *module = sema_project_add_module_at(sema_module_project(sema), node->import.path);
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
		arg->decl = &sema_module_push_decl(sema, sema_scope_decl_new_value(arg->name, type, false))->value_decl;
	}
}

void sema_ast_module_node(SemaModule *sema, AstModuleNode *node) {
	sema_module_push_scope(sema);
	switch (node->type) {
		case AST_MODULE_NODE_TYPE_ALIAS:
		case AST_MODULE_NODE_EXTERNAL_FUNC:
		case AST_MODULE_NODE_USE:
		case AST_MODULE_NODE_IMPORT:
		case AST_MODULE_NODE_STRUCT_DEF:
			break;

		case AST_MODULE_NODE_CONST:
			sema_const_expr_type(sema, node->constant.expr, sema_expr_ctx_default_of(node->constant.type.sema));
			break;

		case AST_MODULE_NODE_FUNC:
			sema_push_ast_func_info(sema, &node->func_decl.info);
			sema_module_set_returns(sema, node->func_decl.info.returning.sema);
			sema_ast_body(sema, &node->func_decl.body);
			break;	
	}
	sema_module_pop_scope(sema);
}

void sema_module_read_decls(SemaModule *sema) {
	sema_module_push_scope(sema);
	sema_module_push_primitives(sema);
    AstModule *module = sema_module_of(sema);
	for (size_t i = 0; i < vec_len(module->nodes); i++) {
		sema_push_ast_module_node(sema, &module->nodes[i]);
	}
}

void sema_module_analyze(SemaModule *sema) {
    AstModule *module = sema_module_of(sema);
	for (size_t i = 0; i < vec_len(module->nodes); i++) {
		sema_ast_module_node(sema, &module->nodes[i]);
	}
	sema_module_pop_scope(sema);
}
