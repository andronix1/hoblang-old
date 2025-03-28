#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "ast/interface/path.h"
#include "ast/interface/expr.h"
#include "core/location.h"
#include "expr/call.h"
#include "expr/idx.h"
#include "expr/unary.h"
#include "expr/array.h"
#include "expr/unwrap.h"
#include "expr/structure.h"
#include "expr/anon_func.h"
#include "expr/as.h"
#include "expr/binop.h"
#include "expr/inner_path.h"
#include "core/slice/slice.h"

typedef enum {
    AST_EXPR_LOCAL_PATH,
    AST_EXPR_INNER_PATH,
    AST_EXPR_INTEGER,
    AST_EXPR_FP,
    AST_EXPR_CHAR,
    AST_EXPR_STR,
    AST_EXPR_BOOLEAN,
    AST_EXPR_UNARY,
    AST_EXPR_ARRAY,
    AST_EXPR_NULL,
    AST_EXPR_STRUCTURE,
    AST_EXPR_ANON_FUNC,
    AST_EXPR_SCOPE,
    AST_EXPR_IDX,
    AST_EXPR_CALL,
    AST_EXPR_AS,
    AST_EXPR_RET_ON_NULL,
    AST_EXPR_UNWRAP,
    AST_EXPR_BINOP,
} AstExprKind;

typedef struct AstExpr {
    AstExprKind kind;
    FileLocation loc;
    union {
        AstExpr *scope;
        AstExpr *ret_on_null;
        AstPath *local_path;
        AstExprInnerPath inner_path;
        AstExprIdx idx;
        AstExprAs as;
        AstExprBinop binop;
        uint64_t integer;
        long double fp;
        char character;
        Slice str;
        bool boolean;
        AstExprCall call;
        AstExprUnary unary;
        AstExprArray array;
        AstExprUnwrap unwrap;
        AstExprStructure structure;
        AstExprAnonFunc anon_func;
    };
} AstExpr;

static inline AstExpr *ast_expr_new_anon_func(FileLocation loc, AstExprAnonFuncArg *args, AstType *returns, AstBody *body) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_ANON_FUNC;
    result->loc = loc;
    result->anon_func.args = args;
    result->anon_func.returns = returns;
    result->anon_func.body = body;
    return result;
}

static inline AstExpr *ast_expr_new_structure(FileLocation loc, AstPath *path, AstExprStructureField *fields) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_STRUCTURE;
    result->loc = loc;
    result->structure.fields = fields;
    result->structure.path = path;
    return result;
}

static inline AstExpr *ast_expr_new_unwrap(FileLocation loc, AstExpr *inner, Slice output_name) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_UNWRAP;
    result->loc = loc;
    result->unwrap.inner = inner;
    result->unwrap.output_name = output_name;
    return result;
}

static inline AstExpr *ast_expr_new_array(FileLocation loc, AstExpr **values) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_ARRAY;
    result->loc = loc;
    result->array.values = values;
    return result;
}

static inline AstExpr *ast_expr_new_unary(FileLocation loc, AstExprUnary unary) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_UNARY;
    result->loc = loc;
    result->unary = unary;
    return result;
}

static inline AstExpr *ast_expr_new_ret_on_null(FileLocation loc, AstExpr *inner) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_RET_ON_NULL;
    result->loc = loc;
    result->ret_on_null = inner;
    return result;
}

static inline AstExpr *ast_expr_new_scope(FileLocation loc, AstExpr *scope) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_SCOPE;
    result->loc = loc;
    result->scope = scope;
    return result;
}

static inline AstExpr *ast_expr_new_call(FileLocation loc, AstExpr *callable, AstExpr **args) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_CALL;
    result->loc = loc;
    result->call.callable = callable;
    result->call.args = args;
    return result;
}

static inline AstExpr *ast_expr_new_bool(FileLocation loc, bool boolean) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_BOOLEAN;
    result->loc = loc;
    result->boolean = boolean;
    return result;
}

static inline AstExpr *ast_expr_new_character(FileLocation loc, char character) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_CHAR;
    result->loc = loc;
    result->character = character;
    return result;
}

static inline AstExpr *ast_expr_new_fp(FileLocation loc, long double fp) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_FP;
    result->loc = loc;
    result->fp = fp;
    return result;
}

static inline AstExpr *ast_expr_new_str(FileLocation loc, Slice str) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_STR;
    result->loc = loc;
    result->str = str;
    return result;
}

static inline AstExpr *ast_expr_new_null(FileLocation loc) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_NULL;
    result->loc = loc;
    return result;
}

static inline AstExpr *ast_expr_new_integer(FileLocation loc, uint64_t integer) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_INTEGER;
    result->loc = loc;
    result->integer = integer;
    return result;
}

static inline AstExpr *ast_expr_new_binop(FileLocation loc, FileLocation binop_loc, AstExpr *left, AstExpr *right, AstBinopKind binop) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_BINOP;
    result->loc = loc;
    result->binop.left = left;
    result->binop.right = right;
    result->binop.kind = binop;
    result->binop.binop_loc = binop_loc;
    return result;
}

static inline AstExpr *ast_expr_new_as_auto(FileLocation loc, FileLocation as_loc, AstExpr *inner, FileLocation as_auto_loc) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_AS;
    result->loc = loc;
    result->as.inner = inner;
    result->as.kind = AST_EXPR_AS_AUTO;
    result->as.as_auto_loc = as_auto_loc;
    result->as.as_loc = as_loc;
    return result;
}

static inline AstExpr *ast_expr_new_as_type(FileLocation loc, FileLocation as_loc, AstExpr *inner, AstType *type) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_AS;
    result->loc = loc;
    result->as.inner = inner;
    result->as.kind = AST_EXPR_AS_TYPE;
    result->as.type = type;
    result->as.as_loc = as_loc;
    return result;
}

static inline AstExpr *ast_expr_new_idx(FileLocation loc, AstExpr *inner, AstExpr *idx) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_IDX;
    result->loc = loc;
    result->idx.inner = inner;
    result->idx.idx = idx;
    return result;
}

static inline AstExpr *ast_expr_new_inner_path(FileLocation loc, AstExpr *inner, AstPath *path) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_INNER_PATH;
    result->loc = loc;
    result->inner_path.inner = inner;
    result->inner_path.path = path;
    return result;
}

static inline AstExpr *ast_expr_new_local_path(FileLocation loc, AstPath *path) {
    AstExpr *result = malloc(sizeof(AstExpr));
    result->kind = AST_EXPR_LOCAL_PATH;
    result->loc = loc;
    result->local_path = path;
    return result;
}
