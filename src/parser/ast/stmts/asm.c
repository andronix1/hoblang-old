#include "asm.h"

AstInlineAsmArg ast_inline_arg_new_reg(Slice reg) {
    AstInlineAsmArg result = {
        .type = AST_INLINE_ASM_ARG_REGISTER,
        .reg = reg
    };
    return result;
}

AstInlineAsmArg ast_inline_arg_new_value(AstValue value) {
    AstInlineAsmArg result = {
        .type = AST_INLINE_ASM_ARG_VALUE,
        .value = value
    };
    return result;
}

AstInlineAsmArg ast_inline_arg_new_expr(AstExpr *expr) {
    AstInlineAsmArg result = {
        .type = AST_INLINE_ASM_ARG_EXPR,
        .expr = expr
    };
    return result;
}