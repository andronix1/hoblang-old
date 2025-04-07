#include "asm.h"
#include "core/assert.h"
#include "ast/shared/expr.h"
#include "core/not_null.h"
#include "core/vec.h"
#include "asm/x86.h"
#include "sema/interface/value.h"
#include "sema/module.h"
#include "sema/nodes/shared/expr.h"

void sema_inline_asm_check_reg(SemaModule *sema, FileLocation at, Slice name) {
    bool found = false;
    for (size_t i = 0; i < sizeof(regs) / sizeof(regs[0]); i++) {
        if (slice_eq(&regs[i].name, &name)) {
            found = true;
            break;
        }
    }
    if (!found) {
        SEMA_ERROR(at, "register `{slice}` was not found", &name);
    }
}

void sema_analyze_inline_asm(SemaModule *sema, AstInlineAsm *inline_asm) {
    for (size_t i = 0; i < vec_len(inline_asm->clobbers); i++) {
        AstClobber *clobber = &inline_asm->clobbers[i];
        sema_inline_asm_check_reg(sema, clobber->loc, clobber->name);
    }
    for (size_t i = 0; i < vec_len(inline_asm->mnemonics); i++) {
        AstInlineAsmMnemonic *mnemonic = &inline_asm->mnemonics[i];
        for (size_t j = 0; j < vec_len(mnemonic->args); j++) {
            AstInlineAsmArg *arg = &mnemonic->args[j];
            switch (arg->kind) {
                case AST_INLINE_ASM_ARG_REGISTER: {
                    sema_inline_asm_check_reg(sema, arg->loc, arg->reg);
                    break;
                }
                case AST_INLINE_ASM_ARG_EXPR: {
                    sema_analyze_runtime_expr(sema, arg->expr, sema_expr_ctx_new(NULL));
                    break;
                }
                case AST_INLINE_ASM_ARG_ADDRESS: {
                    if (!sema_value_is_var(RET_ON_NULL(sema_analyze_expr(sema, arg->expr, sema_expr_ctx_new(NULL))))) {
                        SEMA_ERROR(arg->expr->loc, "expression inside address asm arg must be variable");
                    }
                    break;
                }
            }
        }
    }
}
