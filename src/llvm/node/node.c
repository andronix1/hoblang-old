#include "node.h"
#include "ast/node.h"
#include "ast/shared/expr.h"
#include "ast/shared/body.h"
#include "sema/loop.h"
#include "core/assert.h"
#include "core/slice/api.h"
#include "llvm/node/body_break.h"
#include "llvm/node/asm.h"
#include "llvm/node/body.h"
#include "llvm/node/expr.h"
#include "llvm/node/func.h"
#include "llvm/node/if.h"
#include "llvm/node/val.h"
#include "llvm/type.h"
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
                    llvm_emit_bb(llvm, node->stmt->ret->sema.body_break);
                    if (node->stmt->ret->value) {
                        LLVMBuildRet(llvm->builder, llvm_expr_get(llvm, node->stmt->ret->value));
                    } else {
                        LLVMBuildRetVoid(llvm->builder);
                    }
                    break;
                case AST_NODE_STMT_EXPR:
                    llvm_expr(llvm, node->stmt->expr);
                    break;
                case AST_NODE_STMT_WHILE: {
                    LLVMBasicBlockRef cond
                        = node->stmt->while_loop->sema.loop->llvm.begin
                        = LLVMAppendBasicBlock(llvm->state.func, "");
                    LLVMBasicBlockRef body = LLVMAppendBasicBlock(llvm->state.func, "");
                    LLVMBasicBlockRef code
                        = node->stmt->while_loop->sema.loop->llvm.end
                        = LLVMAppendBasicBlock(llvm->state.func, "");
                    LLVMBuildBr(llvm->builder, cond);

                    llvm_pos_code(llvm, cond);
                    LLVMBuildCondBr(
                        llvm->builder,
                        llvm_expr_get(llvm, node->stmt->while_loop->cond),
                        body, code
                    );

                    llvm_pos_code(llvm, body);
                    llvm_emit_body(llvm, node->stmt->while_loop->body);
                    if (!node->stmt->while_loop->body->sema.breaks) {
                        LLVMBuildBr(llvm->builder, cond);
                    }

                    llvm_pos_defs(llvm, code);
                    llvm_pos_code(llvm, code);
                    break;
                }
                case AST_NODE_STMT_ASSIGN: {
                    LLVMValueRef to = llvm_expr(llvm, node->stmt->assign.assign_to);
                    LLVMValueRef what = llvm_expr_get(llvm, node->stmt->assign.expr);
                    switch (node->stmt->assign.kind) {
                        case AST_STMT_ASSIGN_DIRECT:
                            LLVMBuildStore(llvm->builder, what, to);
                            break;
                        case AST_STMT_ASSIGN_BINOP: {
                            SemaValue *to_val = node->stmt->assign.assign_to->sema.value;
                            LLVMValueRef to_load = LLVMBuildLoad2(
                                llvm->builder,
                                llvm_type(sema_value_is_runtime(to_val)),
                                to, ""
                            );
                            LLVMBuildStore(llvm->builder, llvm_binop(llvm, to_load, what, node->stmt->assign.binop), to);
                            break;
                        }
                    }
                    break;
                }
                case AST_NODE_STMT_INLINE_ASM:
                    llvm_inline_asm(llvm, node->stmt->inline_asm);
                    break;
                case AST_NODE_STMT_IF: {
                    llvm_if(llvm, node->stmt->if_else);
                    break;
                }
                case AST_NODE_STMT_DEFER:
                    break;
                case AST_NODE_STMT_BREAK:
                    llvm_emit_bb(llvm, node->stmt->break_loop->sema.body_break);
                    LLVMBuildBr(llvm->builder, node->stmt->break_loop->sema.loop->llvm.end);
                    break;
                case AST_NODE_STMT_CONTINUE:
                    llvm_emit_bb(llvm, node->stmt->continue_loop->sema.body_break);
                    LLVMBuildBr(llvm->builder, node->stmt->continue_loop->sema.loop->llvm.begin);
                    break;
            }
            break;
    }
}

