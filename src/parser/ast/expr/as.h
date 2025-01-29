#pragma once

#include "../type.h"

typedef enum {
	SEMA_AS_CONV_EXTEND,
	SEMA_AS_CONV_TRUNC,
	SEMA_AS_CONV_BITCAST,

	SEMA_AS_CONV_ARR_TO_SLICE,
	SEMA_AS_CONV_SLICE_TO_PTR,
} SemaAsConvType;

typedef struct {
	AstType type;
	struct _AstExpr *expr;

	SemaAsConvType conv_type;
} AstExprAs;
