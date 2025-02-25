#pragma once 

#include <stdbool.h>
#include "core/location.h"
#include "core/slice.h"
#include "ast/api/expr.h"

typedef enum {
    AST_ASM_ARG_REGISTER,
    AST_ASM_ARG_EXPR,
    AST_ASM_ARG_ADDRESS,
} AstAsmArgType;

typedef struct {
    AstAsmArgType type;
    FileLocation loc;
    union {
        Slice reg;
        AstExpr *expr;
    };
} AstAsmArg;

AstAsmArg ast_asm_reg(Slice reg);
AstAsmArg ast_asm_expr(AstExpr *expr);
AstAsmArg ast_asm_address(AstExpr *expr);

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
