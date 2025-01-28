#pragma once 

#include "core/slice.h"
#include "../expr.h"
#include "../value.h"

typedef enum {
    AST_INLINE_ASM_ARG_REGISTER,
    AST_INLINE_ASM_ARG_VALUE,
    AST_INLINE_ASM_ARG_EXPR,
} AstInlineAsmArgType;

typedef struct {
    AstInlineAsmArgType type;
    union {
        Slice reg;
        AstValue value;
        AstExpr *expr;
    };
} AstInlineAsmArg;

AstInlineAsmArg ast_inline_arg_new_reg(Slice reg);
AstInlineAsmArg ast_inline_arg_new_value(AstValue value);
AstInlineAsmArg ast_inline_arg_new_expr(AstExpr *expr);

typedef struct {
    Slice name;
    AstInlineAsmArg *args;
} AstInlineAsmMnemonic;

typedef struct {
    AstInlineAsmMnemonic *mnems;
} AstInlineAsm;