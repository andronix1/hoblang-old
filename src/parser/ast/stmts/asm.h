#pragma once 

#include "core/slice.h"
#include "../expr.h"
#include "../value.h"

typedef enum {
    AST_INLINE_ASM_ARG_INT,
    AST_INLINE_ASM_ARG_REGISTER,
    AST_INLINE_ASM_ARG_LANG,
} AstInlineAsmArgType;

typedef enum {
    AST_INLINE_ASM_LANG_ARG_VALUE,
    AST_INLINE_ASM_LANG_ARG_EXPR,
} AstInlineAsmLangArgType;

typedef struct {
    AstInlineAsmLangArgType type;
    bool has_constraint;
    Slice constraint;
    union {
        AstValue value;
        AstExpr *expr;
    };
} AstInlineAsmLangArg;

typedef struct {
    AstInlineAsmArgType type;
    union {
        uint64_t integer;
        Slice reg;
        AstInlineAsmLangArg lang;
    };
} AstInlineAsmArg;

AstInlineAsmArg ast_asm_reg(Slice reg);
AstInlineAsmArg ast_asm_int(uint64_t value);
void ast_inline_arg_init_value(AstInlineAsmLangArg *arg, AstValue value);
void ast_inline_arg_init_expr(AstInlineAsmLangArg *arg, AstExpr *expr);

typedef struct {
    Slice name;
    AstInlineAsmArg *args;
} AstInlineAsmMnemonic;

typedef struct {
    Slice *clobbers;
    AstInlineAsmMnemonic *mnems;
} AstInlineAsm;
