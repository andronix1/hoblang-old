#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "core/location.h"
#include "path.h"
#include "expr/as.h"
#include "expr/binop.h"
#include "expr/unary.h"
#include "expr/call.h"
#include "expr/get_local.h"
#include "expr/get_inner.h"
#include "expr/idx.h"
#include "expr/struct.h"
#include "expr/anon_fun.h"
#include "sema/module/decls/decls.h"

typedef enum {
	AST_EXPR_GET_LOCAL_PATH,
	AST_EXPR_GET_INNER_PATH,
	AST_EXPR_IDX,
	AST_EXPR_INTEGER,
	AST_EXPR_FLOAT,
	AST_EXPR_CHAR,
	AST_EXPR_STR,
	AST_EXPR_BOOL,
	AST_EXPR_CALL,
	AST_EXPR_NOT,
	AST_EXPR_AS,
	AST_EXPR_BINOP,
	AST_EXPR_UNARY,
	AST_EXPR_ARRAY,
	AST_EXPR_REF,
	AST_EXPR_NULL,
	AST_EXPR_RET_ON_NULL,
	AST_EXPR_UNWRAP,
	AST_EXPR_STRUCT,
	AST_EXPR_ANON_FUN,
} AstExprType;

typedef struct AstExpr AstExpr;

typedef enum {
    SEMA_NULL_POINTER,
    SEMA_NULL_OPTIONAL
} SemaNullType;

typedef enum {
    SEMA_EXPR_UNWRAP_OPT
} SemaExprUnwrapType;

typedef struct {
    AstExpr *expr;
    Slice name;

    SemaExprUnwrapType type;
    SemaDecl *decl;
} AstExprUnwrap;

typedef struct {
    AstExpr *expr;
    SemaType *fret;
} AstExprRetOnNull;

typedef struct AstExpr {
	AstExprType type;
	FileLocation loc;
    bool scoped;

	union {
		AstExprBinop binop;
		AstCall call;
		AstExprUnary unary;
		AstExprAs as;
		AstExprIdx idx;
		AstExpr **array;
		AstExpr *ref_expr;
		AstExpr *not_expr;
        AstExprRetOnNull ret_on_null;
        AstExprAnonFun anon_fun;
        AstExprUnwrap unwrap;
		AstExprGetLocal get_local;
		AstExprGetInner get_inner;
        AstExprStruct structure;
		Slice str;
		uint64_t integer;
		long double float_value;
		char character;
		bool boolean;
		SemaNullType null_type;
	};
	// for sema
	SemaValue *value;
} AstExpr;
