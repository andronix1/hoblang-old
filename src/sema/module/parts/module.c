#include "ast/private/module_node.h"
#include "core/location.h"
#include "sema/module/behaviour/impl.h"
#include "sema/module/decls/decls.h"
#include "sema/module/parts/decls/behaviour.h"
#include "sema/module/parts/func_info.h"
#include "sema/module/parts/val_decl.h"
#include "sema/module/scopes/api.h"
#include "sema/module/scopes/scope.h"
#include "sema/project/api.h"
#include "sema/module/private.h"
#include "sema/module/api.h"
#include "sema/module/decls/impl.h"
#include "sema/type/private.h"
#include "core/vec.h"
#include "ast/private/module.h"
#include "sema/module/parts/path.h"
#include "sema/module/parts/body.h"
#include "sema/module/parts/type.h"
#include "sema/module/decls/api.h"
#include "sema/value/private.h"
#include "core/assert.h"

void sema_add_ast_func_info(SemaModule *sema, FileLocation at, bool public, AstFuncInfo *info) {	
    SemaType *ext_of = NULL;
	if (info->is_extension) {
        if (!(ext_of = sema_ast_type(sema, info->ext.of))) {
            return;
        }
	}

    SemaType *func_type = sema_ast_func(sema, at, ext_of, info->type.args, info->type.returning);

	info->decl = sema_module_push_module_decl(sema, at, public, sema_decl_new_in_type(
		info->name,
        info->is_extension ? info->ext.of->sema : NULL,
        sema_value_final(func_type)
	));
}

void sema_push_ast_module_node(SemaModule *sema, AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_BEHAVIOUR_DECL: {
            SemaScopeStack new_ss = sema_ss_new(NULL);
            SemaScopeStack *ss = sema_module_ss_swap(sema, &new_ss);
            SemaBehaviour *behaviour = sema_analyze_behaviour_decl(sema, &node->behaviour_decl);
            sema_module_ss_swap(sema, ss);
            if (!behaviour) {
                break;
            }
            node->behaviour_decl.sema.decl = sema_module_push_module_decl(sema, node->loc, node->public, sema_decl_new(node->behaviour_decl.name, sema_value_behaviour(behaviour)));
            break;
        }
		case AST_MODULE_NODE_EXTERNAL_VAR: {
			SemaType *type = sema_ast_type(sema, node->ext_var_decl.type);
			if (!type) {
				break;
			}
            node->ext_var_decl.sema.decl = sema_module_push_module_decl(sema, node->loc, node->public, sema_decl_new(node->ext_var_decl.ext_name, sema_value_var(type)));
            break;
        }
		case AST_MODULE_NODE_TYPE_ALIAS: {
			SemaType *type = sema_ast_type(sema, node->type_alias.type);
			if (!type) {
				break;
			}
			sema_module_push_module_decl(sema, node->loc, node->public, sema_decl_new(node->type_alias.alias, sema_value_type(type)));
			break;
		}
		case AST_MODULE_NODE_STRUCT_DEF: {
            node->struct_def.module = sema;
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
			sema_module_push_module_decl(sema, node->loc, node->public, sema_decl_new(node->struct_def.name, sema_value_type(sema_type_new_struct(&node->struct_def))));
			break;
		}
		case AST_MODULE_NODE_VAL_DECL: {
            SemaDecl *decl = sema_val_decl(sema, &node->val_decl, true);
            if (!decl) {
                break;
            }

            sema_module_push_module_decl(sema, node->loc, node->public, decl);
			break;
		}

		case AST_MODULE_NODE_FROM_USE: {
			SemaDecl *decl = sema_resolve_decl_path(sema, node->from_use.module_path);
            if (!decl) {
                break;
            }
            if (decl->value->type != SEMA_VALUE_MODULE) {
                SEMA_ERROR(node->from_use.module_path->segments[0].loc, "path is not a module");
                break;
            }
            switch (node->from_use.type) {
                case AST_FROM_USE_ALL:
                    sema_module_append_all_from(sema, node->loc, node->public, decl->value->module);
                    break;
                case AST_FROM_USE_LIST:
                    for (size_t i = 0; i < vec_len(node->from_use.items); i++) {
                        AstFromUseListItem *item = &node->from_use.items[i];
                        SemaDecl *item_decl = sema_module_resolve_public_decl(decl->value->module, &item->what);
                        if (!item_decl) {
                            SEMA_ERROR(item->loc, "`{slice}` is undefined", &item->what);
                            continue;
                        }
                        sema_module_push_module_decl(sema, item->loc, node->public, item_decl);
                    }
                    break;
            }
			// TODO: alias
			break;
		}

		case AST_MODULE_NODE_USE: {
			SemaDecl *decl = sema_resolve_decl_path(sema, node->use.path);
            if (!decl) {
                break;
            }
			sema_module_push_module_decl(sema, node->loc, node->public, decl);
            if (decl->value->type == SEMA_VALUE_MODULE) {
                sema_module_append_ext_funcs_from(sema, node->loc, decl->value->module);
            }
			// TODO: alias
			break;
		}

		case AST_MODULE_NODE_IMPORT: {
			SemaModule *module = sema_project_add_module_at(sema_module_project(sema), node->import.path);
			if (!module) {
				break;
			}

			sema_module_push_module_decl(sema, node->loc, node->public, sema_decl_new(node->import.as, sema_value_module(module)));
            sema_module_append_ext_funcs_from(sema, node->loc, module);
			break;
		}

		case AST_MODULE_NODE_FUNC:
			sema_add_ast_func_info(sema, node->loc, node->public, &node->func_decl.info);
			break;
		
		case AST_MODULE_NODE_EXTERNAL_FUNC:
			sema_add_ast_func_info(sema, node->loc, node->public, &node->ext_func_decl.info);
			break;
	}
}

void sema_push_ast_func_info(SemaModule *sema, FileLocation at, AstFuncInfo *info) {
	if (info->is_extension) {
        // TODO: self is final
		info->self = sema_module_push_scope_decl(sema, at, sema_decl_new(slice_from_cstr("self"), sema_value_var(info->ext.of->sema)));
	}
	for (size_t i = 0; i < vec_len(info->type.args); i++) {
		AstFuncArg *arg = &info->type.args[i];
		SemaType *type = sema_ast_type(sema, arg->type);
		arg->decl = sema_module_push_scope_decl(sema, at, sema_decl_new(arg->name, sema_value_var(type)));
	}
}

void sema_ast_module_node(SemaModule *sema, AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_BEHAVIOUR_DECL:
		case AST_MODULE_NODE_TYPE_ALIAS:
		case AST_MODULE_NODE_VAL_DECL:
		case AST_MODULE_NODE_EXTERNAL_FUNC:
		case AST_MODULE_NODE_EXTERNAL_VAR:
		case AST_MODULE_NODE_USE:
		case AST_MODULE_NODE_FROM_USE:
		case AST_MODULE_NODE_IMPORT:
		case AST_MODULE_NODE_STRUCT_DEF:
			break;

		case AST_MODULE_NODE_FUNC: {
            SemaScopeStack new_ss = sema_ss_new(node->func_decl.info.type.returning->sema);
            SemaScopeStack *ss = sema_module_ss_swap(sema, &new_ss);
            sema_module_push_scope(sema);
			sema_push_ast_func_info(sema, node->loc, &node->func_decl.info);
			sema_ast_body(sema, &node->func_decl.body);
            sema_module_pop_scope(sema);
            sema_module_ss_swap(sema, ss);
			break;
        }
	}
}

void sema_module_read_decls(SemaModule *sema) {
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
}
