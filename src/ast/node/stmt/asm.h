#pragma once

#include <malloc.h>
#include "ast/interface/expr.h"
#include "core/location.h"
#include "core/slice/slice.h"

typedef struct {
    Slice name;
    FileLocation loc;
} AstClobber;

typedef enum {
    AST_INLINE_ASM_ARG_REGISTER,
    AST_INLINE_ASM_ARG_EXPR,
    AST_INLINE_ASM_ARG_ADDRESS,
} AstInlineAsmArgKind;

typedef struct {
    AstInlineAsmArgKind kind;
    FileLocation loc;
    union {
        Slice reg;
        AstExpr *address_expr;
        AstExpr *expr;
    };
} AstInlineAsmArg;

typedef struct {
    Slice name;
    AstInlineAsmArg *args;
} AstInlineAsmMnemonic;

typedef enum {
    AST_INLINE_ASM_VOLATILE = 1 << 0
} AstInlineAsmFlags;

typedef struct {
    AstInlineAsmFlags flags;
    AstClobber *clobbers;
    AstInlineAsmMnemonic *mnemonics;
} AstInlineAsm;

static inline AstInlineAsmArg ast_inline_asm_arg_new_address(FileLocation loc, AstExpr *expr) {
    AstInlineAsmArg result;
    result.kind = AST_INLINE_ASM_ARG_ADDRESS;
    result.address_expr = expr;
    result.loc = loc;
    return result;
}

static inline AstInlineAsmArg ast_inline_asm_arg_new_expr(FileLocation loc, AstExpr *expr) {
    AstInlineAsmArg result;
    result.kind = AST_INLINE_ASM_ARG_EXPR;
    result.expr = expr;
    result.loc = loc;
    return result;
}

static inline AstInlineAsmArg ast_inline_asm_arg_new_reg(FileLocation loc, Slice name) {
    AstInlineAsmArg result;
    result.kind = AST_INLINE_ASM_ARG_REGISTER;
    result.reg = name;
    result.loc = loc;
    return result;
}

static inline AstInlineAsmMnemonic ast_inline_asm_mnemonic_new(Slice name, AstInlineAsmArg *args) {
    AstInlineAsmMnemonic result;
    result.name = name;
    result.args = args;
    return result;
}

static inline AstClobber ast_inline_asm_clobber_new(Slice name, FileLocation loc) {
    AstClobber result = {
        .name = name,
        .loc = loc
    };
    return result;
}

static inline AstInlineAsm *ast_inline_asm_new(AstInlineAsmFlags flags, AstClobber *clobbers, AstInlineAsmMnemonic *mnemonics) {
    AstInlineAsm *result = malloc(sizeof(AstInlineAsm));
    result->clobbers = clobbers;
    result->flags = flags;
    result->mnemonics = mnemonics;
    return result;
}
