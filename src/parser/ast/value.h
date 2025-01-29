#pragma once

#include "mod_path.h"

typedef enum {
	AST_VALUE_IDENT,
	AST_VALUE_IDX,
	AST_VALUE_DEREF
} AstValueType;

struct _SemaType;

typedef enum {
	SEMA_VALUE_GET_STRUCT,
	SEMA_VALUE_GET_SLICE,
} SemaValueGetIdentType;

typedef enum {
	SEMA_VALUE_SLICE_MEMBER_LENGTH,
	SEMA_VALUE_SLICE_MEMBER_POINTER
} SemaValueSliceMember;

typedef struct {
	SemaValueSliceMember member;
	struct _SemaType *slice_of;
} SemaValueGetSliceIdent;

typedef struct {
	size_t member_idx;
	struct _SemaType *sema_type;
} SemaValueGetStructIdent;

typedef struct {
	Slice ident;

	// sema
	SemaValueGetIdentType get_type;
	union {
		SemaValueGetStructIdent struct_ident;
		SemaValueGetSliceIdent slice_ident;
	};
} AstValueIdent;

typedef struct {
	struct _SemaType *sema_type;
	AstValueType type;
	union {
		AstValueIdent ident;
		struct _AstExpr *idx;
	};
} AstValueSegment;

typedef struct {
	struct _SemaType *sema_type;
	AstModPath mod_path;
	AstValueSegment *segments;
} AstValue;
