#include "../../../parts.h"

void llvm_stmt_asm(LlvmBackend *llvm, AstInlineAsm *inline_asm) {
    char *assembly = vec_new(char);
    char *constraint = vec_new(char);
    size_t args_count = 0;
    bool has_side_effect = true;
    char int_buf[16] = {0};
    LLVMValueRef *values = vec_new(LLVMValueRef);
    LLVMTypeRef *types = vec_new(LLVMTypeRef);

    for (size_t i = 0; i < vec_len(inline_asm->mnems); i++) {
        AstInlineAsmMnemonic *mnemonic = &inline_asm->mnems[i];
        assembly = vec_append_raw(assembly, mnemonic->name.str, mnemonic->name.len);
        assembly = vec_append_raw(assembly, " ", 1);
        for (size_t j = 0; j < vec_len(mnemonic->args); j++) {
            if (j != 0) {
                assembly = vec_append_raw(assembly, ",", 1);
            }
            // TODO: output, clobber
            AstInlineAsmArg *arg = &mnemonic->args[j];
            switch (arg->type) {
                case AST_INLINE_ASM_ARG_REGISTER: {
                    assembly = vec_append_raw(assembly, arg->reg.str, arg->reg.len);
                    break;
                }
                
                case AST_INLINE_ASM_ARG_EXPR: {
                    LLVMValueRef value = llvm_expr(llvm, arg->expr);
                    values = vec_push(values, &value);
                    LLVMTypeRef type = llvm_resolve_type(arg->expr->sema_type);
                    types = vec_push(types, &type);
                    if (args_count > 0) {
                        constraint = vec_append_raw(constraint, ",", 1);
                    }
                    constraint = vec_append_raw(constraint, "X", 1);
                    assembly = vec_append_raw(assembly, int_buf, sprintf(int_buf, "$%ld", args_count++));
                    hob_log(LOGW, "TODO: check expression type from regs");
                    break;
                }

                case AST_INLINE_ASM_ARG_VALUE: {
                    LLVMValueRef value = llvm_value(llvm, &arg->value);
                    values = vec_push(values, &value);
                    LLVMTypeRef type = llvm_resolve_type(arg->value.sema_type);
                    types = vec_push(types, &type);
                    if (args_count > 0) {
                        constraint = vec_append_raw(constraint, ",", 1);
                    }
                    constraint = vec_append_raw(constraint, "X", 1);
                    assembly = vec_append_raw(assembly, int_buf, sprintf(int_buf, "$%ld", args_count++));
                    break;
                }
            }
        }
        assembly = vec_append_raw(assembly, "\n", 1);
    }

    LLVMValueRef call = LLVMGetInlineAsm(
        LLVMVoidType(),
        assembly,
        vec_len(assembly),
        constraint,
        vec_len(constraint),
        has_side_effect,
        true, // required_aligned_stack
        LLVMInlineAsmDialectIntel,
        false
    );
    LLVMSetVolatile(call, true);
    LLVMBuildCall2(llvm->builder, LLVMFunctionType(LLVMVoidType(), types, vec_len(types), false), call, values, vec_len(values), "");
}