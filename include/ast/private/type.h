#pragma once

#include "core/slice.h"
#include "sema/type/type.h"
#include "ast/api/type.h"
#include "ast/api/module_node.h"
#include "path.h"
#include "ast/api/expr.h"

typedef enum {
	AST_TYPE_PATH,
	AST_TYPE_FUNCTION,
	AST_TYPE_SLICE,
	AST_TYPE_ARRAY,
	AST_TYPE_OPTIONAL,
	AST_TYPE_POINTER
} AstTypeKind;

typedef struct AstFunctionType {
	AstType **args;
	AstType *returns;
} AstFunctionType;

typedef struct {
	AstType *of;
	AstExpr *length;
} AstArrayType;

typedef struct AstType {
	AstTypeKind type;
	union {
		AstPath *path;
		AstFunctionType func;
		AstType *ptr_to;
		AstType *slice_of;
		AstType *optional_of;
		AstArrayType array;
	};
	SemaType *sema;
} AstType;
