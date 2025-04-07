#include "node.h"
#include "ast/node.h"
#include "ast/node/decl.h"
#include "sema/decl.h"
#include "sema/module.h"
#include "sema/nodes/decl/behaviour.h"
#include "sema/nodes/decl/from_use.h"
#include "sema/nodes/decl/func.h"
#include "sema/nodes/decl/global.h"
#include "sema/nodes/decl/import.h"
#include "sema/nodes/decl/struct.h"
#include "sema/nodes/decl/type.h"
#include "sema/nodes/decl/use.h"
#include "sema/nodes/decl/val.h"
#include "sema/nodes/shared/expr.h"
#include "sema/nodes/shared/generic.h"
#include "sema/nodes/shared/global.h"
#include "sema/nodes/shared/type.h"
#include "ast/shared/func_info.h"
#include "sema/nodes/stmt/asm.h"
#include "sema/nodes/stmt/assign.h"
#include "sema/nodes/stmt/if.h"
#include "sema/nodes/stmt/return.h"
#include "sema/nodes/stmt/while.h"
#include "sema/scope.h"
#include "sema/value.h"

void sema_module_analyze_node_stmt(SemaModule *sema, FileLocation loc, AstNodeStmt *stmt) {
    if (sema_module_is_global(sema)) {
        SEMA_ERROR(loc, "statements are not allowed in global scope");
        return;
    }
    switch (stmt->kind) {
        case AST_NODE_STMT_IF: sema_analyze_stmt_if(sema, stmt->if_else); break;
        case AST_NODE_STMT_RETURN: sema_analyze_stmt_return(sema, loc, stmt->ret); break;
        case AST_NODE_STMT_WHILE: sema_analyze_stmt_while(sema, stmt->while_loop); break;
        case AST_NODE_STMT_INLINE_ASM: sema_analyze_inline_asm(sema, stmt->inline_asm); break;
        case AST_NODE_STMT_DEFER: SEMA_ERROR(loc, "NIY"); break;
        case AST_NODE_STMT_EXPR: sema_analyze_expr(sema, stmt->expr, sema_expr_ctx_new(NULL)); break;
        case AST_NODE_STMT_BREAK: SEMA_ERROR(loc, "NIY"); break;
        case AST_NODE_STMT_CONTINUE: SEMA_ERROR(loc, "NIY"); break;
        case AST_NODE_STMT_ASSIGN: sema_analyze_stmt_assign(sema, &stmt->assign); break;
    }
}

void sema_module_read_node_decl(SemaModule *sema, AstNodeDecl *decl) {
    switch (decl->kind) {
        case AST_NODE_DECL_GENERIC: {
            bool is_generic = decl->generic.generics != NULL;
            SemaScopeStack ss;
            if (is_generic) {
                ss = sema_scope_stack_new();
                sema_module_swap_ss(sema, &ss);
                sema_module_push_generics(sema, decl->generic.generics);
            }
            switch (decl->generic.kind) {
                case AST_NODE_DECL_GENERIC_FUNC: sema_module_read_func(sema, decl->generic.func_decl, is_generic, decl->public); break;
                case AST_NODE_DECL_GENERIC_STRUCT: sema_module_read_struct(sema, &decl->generic.struct_decl, is_generic, decl->public); break;
                case AST_NODE_DECL_GENERIC_TYPE_ALIAS: sema_module_read_type_alias(sema, &decl->generic.type_alias, is_generic, decl->public); break;
                case AST_NODE_DECL_GENERIC_BEHAVIOUR: sema_module_read_behaviour(sema, &decl->generic.behaviour, is_generic, decl->public); break;
            }
            if (is_generic) {
                sema_module_pop_scope(sema);
                sema_module_swap_ss(sema, NULL);
                sema_module_push_decl(sema, decl->decl_loc, sema_decl_new_in_type(
                    ast_node_decl_generic_get_name(&decl->generic),
                    decl->generic.kind == AST_NODE_DECL_GENERIC_FUNC ?
                        (decl->generic.func_decl->info->ext_of ? 
                            sema_resolve_type(sema, decl->generic.func_decl->info->ext_of) :
                            NULL
                        ) : NULL,
                    sema_value_new_comp_time(sema_value_comp_time_new_generic(&decl->generic))), decl->public);
            }
            break;
        }
        case AST_NODE_DECL_VAL: sema_module_read_val_decl(sema, decl->val, decl->public); break;
        case AST_NODE_DECL_EXTERNAL: sema_module_read_global_decl_info(sema, decl->external, decl->public); break;
        case AST_NODE_DECL_GLOBAL: sema_module_read_global_decl(sema, &decl->global_decl, decl->public); break;
        case AST_NODE_DECL_USE: sema_module_read_use(sema, &decl->use, decl->public); break;
        case AST_NODE_DECL_FROM_USE: sema_module_read_from_use(sema, &decl->from_use, decl->public); break;
        case AST_NODE_DECL_IMPORT: sema_module_read_import(sema, &decl->import, decl->public); break;
    }
}

void sema_module_analyze_node_decl(SemaModule *sema, FileLocation loc, AstNodeDecl *decl) {
    if (!sema_module_is_global(sema)) {
        sema_module_read_node_decl(sema, decl);
    }
    switch (decl->kind) {
        case AST_NODE_DECL_GENERIC:
            switch (decl->generic.kind) {
                case AST_NODE_DECL_GENERIC_FUNC:
                    sema_analyze_func_decl(sema, decl->generic.func_decl);
                    break;
                case AST_NODE_DECL_GENERIC_STRUCT:
                case AST_NODE_DECL_GENERIC_TYPE_ALIAS:
                case AST_NODE_DECL_GENERIC_BEHAVIOUR:
                    break;
            }
            break;

        case AST_NODE_DECL_GLOBAL:
            sema_analyze_func_decl(sema, decl->global_decl.func_decl);
            break;
        case AST_NODE_DECL_VAL:
        case AST_NODE_DECL_EXTERNAL:
        case AST_NODE_DECL_USE:
        case AST_NODE_DECL_FROM_USE:
        case AST_NODE_DECL_IMPORT:
            break;
    }
}

void sema_module_read_node(SemaModule *sema, AstNode *node) {
    switch (node->kind) {
        case AST_NODE_DECL: sema_module_read_node_decl(sema, node->decl); break;
        case AST_NODE_STMT: break;
    }
}

void sema_module_analyze_node(SemaModule *sema, AstNode *node) {
    switch (node->kind) {
        case AST_NODE_DECL: sema_module_analyze_node_decl(sema, node->loc, node->decl); break;
        case AST_NODE_STMT: sema_module_analyze_node_stmt(sema, node->loc, node->stmt); break;
    }
}
