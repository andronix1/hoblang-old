#include "node.h"
#include "ast/node.h"
#include "core/assert.h"
#include "core/slice/api.h"
#include "llvm/node/expr.h"
#include "llvm/node/func.h"
#include "llvm/node/val.h"
#include <llvm-c/Core.h>

void llvm_setup_node(LlvmBackend *llvm, AstNode *node) {
    switch (node->kind) {
        case AST_NODE_DECL: 
            switch (node->decl->kind) {
                case AST_NODE_DECL_GENERIC: {
                    AstNodeDeclGeneric *generic = &node->decl->generic;
                    if (generic->generics) {
                        assert(0, "Generics are NIY");
                    }
                    switch (generic->kind) {
                        case AST_NODE_DECL_GENERIC_FUNC:
                            llvm_setup_func_decl(llvm, generic->func_decl, "");
                            break;
                        case AST_NODE_DECL_GENERIC_STRUCT:
                        case AST_NODE_DECL_GENERIC_TYPE_ALIAS:
                        case AST_NODE_DECL_GENERIC_BEHAVIOUR:
                            break;
                    }
                    break;
                }
                case AST_NODE_DECL_VAL:
                    llvm_setup_val_decl(llvm, node->decl->val, "");
                    break;
                case AST_NODE_DECL_GLOBAL: {
                    AstGlobalDecl *global = &node->decl->global_decl;
                    switch (global->kind) {
                        case AST_GLOBAL_DECL_FUNC:
                            llvm_setup_func_decl(llvm, global->func_decl, slice_to_cstr(&global->public_name));
                            break;
                        case AST_GLOBAL_DECL_VAL:
                            llvm_setup_val_decl(llvm, global->val_decl, slice_to_cstr(&global->public_name));
                            break;
                    }
                    break;
                }
                case AST_NODE_DECL_EXTERNAL: {
                    AstGlobalDeclInfo *global = node->decl->external;
                    switch (global->kind) {
                        case AST_GLOBAL_DECL_INFO_FUNC:
                            llvm_setup_func_info(llvm, global->func, slice_to_cstr(&global->public_name));
                            break;
                        case AST_GLOBAL_DECL_INFO_VAL:
                            llvm_setup_func_info(llvm, global->func, slice_to_cstr(&global->public_name));
                            break;
                    }
                    break;
                }
                case AST_NODE_DECL_USE:
                case AST_NODE_DECL_FROM_USE:
                case AST_NODE_DECL_IMPORT:
                    break;
            }
        case AST_NODE_STMT: break;
    }
}

void llvm_emit_node(LlvmBackend *llvm, AstNode *node) {
    switch (node->kind) {
        case AST_NODE_DECL: 
            switch (node->decl->kind) {
                case AST_NODE_DECL_GENERIC: {
                    AstNodeDeclGeneric *generic = &node->decl->generic;
                    if (generic->generics) {
                        assert(0, "Generics are NIY");
                    }
                    switch (generic->kind) {
                        case AST_NODE_DECL_GENERIC_FUNC:
                            llvm_emit_func_decl(llvm, generic->func_decl);
                            break;
                        case AST_NODE_DECL_GENERIC_STRUCT:
                        case AST_NODE_DECL_GENERIC_TYPE_ALIAS:
                        case AST_NODE_DECL_GENERIC_BEHAVIOUR:
                            break;
                    }
                    break;
                }
                case AST_NODE_DECL_VAL:
                    llvm_emit_val_decl(llvm, node->decl->val);
                    break;
                case AST_NODE_DECL_GLOBAL: {
                    AstGlobalDecl *global = &node->decl->global_decl;
                    switch (global->kind) {
                        case AST_GLOBAL_DECL_FUNC:
                            llvm_emit_func_decl(llvm, global->func_decl);
                            break;
                        case AST_GLOBAL_DECL_VAL:
                            llvm_emit_val_decl(llvm, global->val_decl);
                            break;
                    }
                    break;
                }
                case AST_NODE_DECL_EXTERNAL:
                case AST_NODE_DECL_USE:
                case AST_NODE_DECL_FROM_USE:
                case AST_NODE_DECL_IMPORT:
                    break;
            }
            break;
        case AST_NODE_STMT:
            switch (node->stmt->kind) {
                case AST_NODE_STMT_RETURN:
                    LLVMBuildRet(llvm->builder, llvm_expr(llvm, node->stmt->ret->value));
                    break;
                case AST_NODE_STMT_EXPR:
                    llvm_expr(llvm, node->stmt->expr);
                    break;
                case AST_NODE_STMT_IF:
                case AST_NODE_STMT_WHILE:
                case AST_NODE_STMT_INLINE_ASM:
                case AST_NODE_STMT_DEFER:
                case AST_NODE_STMT_BREAK:
                case AST_NODE_STMT_CONTINUE:
                case AST_NODE_STMT_ASSIGN:
                    assert(0, "NIY");
            }
            break;
    }
}

