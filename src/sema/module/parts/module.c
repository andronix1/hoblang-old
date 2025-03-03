#include "core/location.h"
#include "sema/project/api.h"
#include "sema/module/private.h"
#include "sema/module/api.h"
#include "sema/module/decls/impl.h"
#include "sema/type/api.h"
#include "sema/type/private.h"
#include "core/vec.h"
#include "ast/private/module.h"
#include "ast/private/expr.h"
#include "sema/module/parts/path.h"
#include "sema/module/parts/body.h"
#include "sema/module/parts/expr.h"
#include "sema/module/parts/type.h"
#include "sema/module/decls/api.h"
#include "sema/value/private.h"

void sema_add_ast_func_info(SemaModule *sema, FileLocation at, AstFuncInfo *info) {	
	SemaType *returning = sema_ast_type(sema, &info->returning);
	if (!returning) {
		return;
	}

	if (info->is_extension) {
        sema_ast_type(sema, &info->ext.of);
	}

	SemaType **args = vec_new(SemaType*);
	if (info->is_extension) {
		SemaType *type = info->ext.of.sema;
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
	info->decl = sema_module_push_public_decl(sema, at, sema_decl_new_in_type(
		info->name,
        info->is_extension ? info->ext.of.sema : NULL,
        sema_value_final(sema_type_new_func(returning, args))
	));
}

// TODO: remove duplicate
void sema_stmt_const(SemaModule *sema, FileLocation loc, AstConst *constant);

void sema_push_ast_module_node(SemaModule *sema, AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_TYPE_ALIAS: {
			SemaType *type = sema_ast_type(sema, &node->type_alias.type);
			if (!type) {
				break;
			}
			sema_module_push_public_decl(sema, node->loc, sema_decl_new(node->type_alias.alias, sema_value_type(type)));
			break;
		}
		case AST_MODULE_NODE_STRUCT_DEF: {
			for (size_t i = 0; i < vec_len(node->struct_def.members); i++) {
				AstStructMember *member = &node->struct_def.members[i];
				for (size_t j = 0; j < i; j++) {
					AstStructMember *jmember = &node->struct_def.members[j];
					if (slice_eq(&member->name, &jmember->name)) {
						SEMA_ERROR(member->loc, "field {slice} duplicated", &member->name);
					}
				}
				sema_ast_type(sema, member->type);
			}
			sema_module_push_public_decl(sema, node->loc, sema_decl_new(node->struct_def.name, sema_value_type(sema_type_new_struct(&node->struct_def))));
			break;
		}
		case AST_MODULE_NODE_CONST: {
            sema_stmt_const(sema, node->loc, &node->constant);
			break;
		}

		case AST_MODULE_NODE_USE: {
			SemaDecl *decl = sema_resolve_decl_path(sema, &node->use.path);
            if (!decl) {
                break;
            }
			sema_module_push_public_decl(sema, node->loc, decl);
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

			sema_module_push_public_decl(sema, node->loc, sema_decl_new(node->import.as, sema_value_module(module)));
            sema_module_append_ext_funcs_from(sema, node->loc, module);
			break;
		}

		case AST_MODULE_NODE_FUNC:
			sema_add_ast_func_info(sema, node->loc, &node->func_decl.info);
			break;
		
		case AST_MODULE_NODE_EXTERNAL_FUNC:
			sema_add_ast_func_info(sema, node->loc, &node->ext_func_decl.info);
			break;
	}
}

void sema_push_ast_func_info(SemaModule *sema, FileLocation at, AstFuncInfo *info) {
	if (info->is_extension) {
        // TODO: self is final
		info->self = sema_module_push_decl(sema, at, sema_decl_new(slice_from_cstr("self"), sema_value_var(info->ext.of.sema)));
	}
	for (size_t i = 0; i < vec_len(info->args); i++) {
		AstFuncArg *arg = &info->args[i];
		SemaType *type = sema_ast_type(sema, &arg->type);
		arg->decl = sema_module_push_decl(sema, at, sema_decl_new(arg->name, sema_value_var(type)));
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
			sema_const_expr(sema, node->constant.expr, sema_expr_ctx_default_of(node->constant.type.sema));
			break;

		case AST_MODULE_NODE_FUNC:
			sema_push_ast_func_info(sema, node->loc, &node->func_decl.info);
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
