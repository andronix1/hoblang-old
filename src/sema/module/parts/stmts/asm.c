#include "../parts.h"
#include "asm/x86.h"

void sema_stmt_inline_asm(SemaModule *sema, AstInlineAsm *inline_asm) {
    for (size_t i = 0; i < vec_len(inline_asm->mnems); i++) {
        AstAsmMnemonic *mnemonic = &inline_asm->mnems[i];
        for (size_t j = 0; j < vec_len(mnemonic->args); j++) {
            AstAsmArg *arg = &mnemonic->args[j];
            switch (arg->type) {
                case AST_ASM_ARG_REGISTER: {
                    bool found = false;
                    for (size_t i = 0; i < sizeof(regs) / sizeof(regs[0]); i++) {
                        if (slice_eq(&regs[i].name, &arg->reg)) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        sema_err("register `{slice}` was not found", &arg->reg);
                    }
                    break;
                }

                case AST_ASM_ARG_EXPR: {
                    sema_value_expr_type(sema, arg->expr, sema_expr_ctx_default());
                    break;
                }
                case AST_ASM_ARG_ADDRESS: {
                    sema_var_expr_type(sema, arg->expr, sema_expr_ctx_default());
                    break;
                }
            }
        }
    }
}
