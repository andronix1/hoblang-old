#include "asm.h"

AstInlineAsmArg ast_asm_reg(Slice reg) {
    AstInlineAsmArg result = {
        .type = AST_INLINE_ASM_ARG_REGISTER,
        .reg = reg
    };
    return result;
}

AstInlineAsmArg ast_asm_int(uint64_t value) {
    AstInlineAsmArg result = {
        .type = AST_INLINE_ASM_ARG_INT,
        .integer = value
    };
    return result;
}

void ast_inline_arg_init_value(AstInlineAsmLangArg *arg, AstValue value) {
    arg->type = AST_INLINE_ASM_LANG_ARG_VALUE;
    arg->value = value;
    arg->has_constraint = false;
}

void ast_inline_arg_init_expr(AstInlineAsmLangArg *arg, AstExpr *expr) {
    arg->type = AST_INLINE_ASM_LANG_ARG_EXPR;
    arg->expr = expr;
    arg->has_constraint = false;
}