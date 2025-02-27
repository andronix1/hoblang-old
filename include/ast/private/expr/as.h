#pragma once

#include "../type.h"
#include "core/location.h"

typedef enum {
	SEMA_AS_CONV_EXTEND,
	SEMA_AS_CONV_TRUNC,
	SEMA_AS_CONV_FEXTEND,
	SEMA_AS_CONV_FTRUNC,
	SEMA_AS_CONV_BITCAST,

	SEMA_AS_CONV_INT_TO_FLOAT,
	SEMA_AS_CONV_UINT_TO_FLOAT,
	SEMA_AS_CONV_FLOAT_TO_INT,
	SEMA_AS_CONV_FLOAT_TO_UINT,
	SEMA_AS_CONV_INT_TO_PTR,
	SEMA_AS_CONV_PTR_TO_INT,

    SEMA_AS_CONV_OPT_UNWRAP,
    SEMA_AS_CONV_OPT_WRAP,

	SEMA_AS_CONV_ARR_PTR_TO_SLICE,
	SEMA_AS_CONV_SLICE_TO_PTR,

	SEMA_AS_CONV_IGNORE,
} SemaAsConvType;

typedef enum {
    AST_EXPR_AS_TYPE,
    AST_EXPR_AS_AUTO
} AstExprAsType;

typedef struct {
    AstExprAsType type;
	AstExpr *expr;
    FileLocation loc;

    union {
	    AstType as_type;
    };

	SemaAsConvType conv_type;
    SemaType *sema_type;
} AstExprAs;
