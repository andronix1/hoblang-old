#pragma once 

#include "core/slice.h"
#include "../expr.h"
#include "../value.h"

typedef enum {
    AST_ASM_ARG_REGISTER,
    AST_ASM_ARG_EXPR,
    AST_ASM_ARG_VALUE,
} AstAsmArgType;

// typedef enum {
//     AST_ASM_CLOBBER_IMM = 1 << 1,
//     AST_ASM_CLOBBER_MEM = 1 << 2,
//     AST_ASM_CLOBBER_REG = 1 << 3,
//     AST_ASM_CLOBBER_ADDR = 1 << 4,
// } AstAsmConstraint;

typedef struct {
    AstAsmArgType type;
    union {
        Slice reg;
        AstExpr *expr;
        AstValue value;
    };
} AstAsmArg;

AstAsmArg ast_asm_reg(Slice reg);
AstAsmArg ast_asm_expr(AstExpr *expr);
AstAsmArg ast_asm_value(AstValue value);

typedef struct {
    Slice name;
    AstAsmArg *args;
} AstAsmMnemonic;

typedef struct {
    // AstAsmClobbers clobbers;
    bool is_volatile;
    Slice *clobbers;
    AstAsmMnemonic *mnems;
} AstInlineAsm;
