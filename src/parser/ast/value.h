#pragma once

#include "mod_path.h"

typedef enum {
	AST_VALUE_IDENT,
	AST_VALUE_IDX,
	AST_VALUE_DEREF
} AstValueType;

struct _SemaType;

typedef struct {
	Slice ident;

	// sema
	size_t struct_member_idx;
	struct _SemaType *struct_sema_type;
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
