#pragma once

#include "core/slice.h"
#include "sema/type/type.h"
#include "ast/api/type.h"
#include "path.h"
#include "ast/api/expr.h"

typedef enum {
	AST_TYPE_PATH,
	AST_TYPE_STRUCT,
	AST_TYPE_FUNCTION,
	AST_TYPE_SLICE,
	AST_TYPE_ARRAY,
	AST_TYPE_POINTER
} AstTypeKind;

typedef struct {
	AstType *args;
	AstType *returns;
} AstFunctionType;

typedef struct {
	Slice name;
	AstType *type;
} AstStructMember;

typedef struct AstStructType {
	AstStructMember *members;
} AstStructType;

typedef struct {
	AstType *of;
	AstExpr *length;
} AstArrayType;

typedef struct AstType {
	AstTypeKind type;
	union {
		AstPath path;
		AstStructType struct_type;
		AstFunctionType func;
		AstType *ptr_to;
		AstType *slice_of;
		AstArrayType array;
	};
	SemaType *sema;
} AstType;
