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
	SemaScopeDecl *ext_decl = NULL;
	if (info->is_extension) {
		ext_decl = sema_module_resolve_scope_decl(sema, &info->extension_of);
		if (ext_decl) {
			if (ext_decl->type != SEMA_SCOPE_DECL_TYPE) {
				sema_err("ext function for types only");
				ext_decl = NULL;
			}
		}
	}

	SemaType **args = vec_new(SemaType*);
	if (ext_decl) {
		SemaType *type = info->ext_type = sema_type_new_pointer(ext_decl->sema_type);
		args = vec_push(args, &type);
	}
	for (size_t i = 0; i < vec_len(info->args); i++) {
		AstFuncArg *arg = &info->args[i];
		SemaType *type = sema_ast_type(sema, &arg->type);
		if (!type) {
			return;
		}
		args = vec_push(args, &type);
	}
	SemaScopeValueDecl *fdecl = &sema_module_push_public_decl(sema, sema_scope_decl_new_value(
		info->name,
		sema_type_new_func(returning, args),
		true
	))->value_decl;
	info->decl = fdecl;
	if (ext_decl) {
		if (ext_decl->sema_type->type == SEMA_TYPE_STRUCT) {
			SemaStructExtFunc func = {
				.decl = fdecl,
				.name = info->name
			};
			ext_decl->sema_type->struct_def->ext_funcs = vec_push(ext_decl->sema_type->struct_def->ext_funcs, &func);
			return;
		}
		sema_err("cannot add ext function for {sema::type}", ext_decl->sema_type);
	}
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
			node->struct_def.ext_funcs = vec_new(SemaStructExtFunc);
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
	if (info->is_extension) {
		info->self = &sema_module_push_decl(sema, sema_scope_decl_new_value(slice_from_cstr("self"), info->ext_type, false))->value_decl;
	}
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
