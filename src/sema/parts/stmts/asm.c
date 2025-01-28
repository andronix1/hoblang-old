#include "../../parts.h"

void sema_stmt_inline_asm(SemaModule *sema, AstInlineAsm *inline_asm) {
    for (size_t i = 0; i < vec_len(inline_asm->mnems); i++) {
        AstInlineAsmMnemonic *mnemonic = &inline_asm->mnems[i];
        for (size_t j = 0; j < vec_len(mnemonic->args); j++) {
            AstInlineAsmArg *arg = &mnemonic->args[j];
            switch (arg->type) {
                case AST_INLINE_ASM_ARG_REGISTER:
                    hob_log(LOGW, "registers are unsafe now :(");
                    break;
                
                case AST_INLINE_ASM_ARG_EXPR:
                    hob_log(LOGW, "TODO: check expression type from regs");
                    sema_ast_expr_type(sema, arg->expr, NULL);
                    break;

                case AST_INLINE_ASM_ARG_VALUE:
                    sema_ast_value(sema, &arg->value);
                    break;
            }
        }
    }
}