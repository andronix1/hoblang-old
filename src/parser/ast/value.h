#pragma once

typedef enum {
	AST_VALUE_IDENT,
	AST_VALUE_IDX,
	AST_VALUE_DEREF
} AstValueType;

struct _SemaType;

typedef struct {
	struct _SemaType *sema_type;
	AstValueType type;
	union {
		Slice ident;
		struct _AstExpr *idx;
	};
} AstValueSegment;

typedef struct {
	struct _SemaType *sema_type;
	AstModPath mod_path;
	AstValueSegment *segments;
} AstValue;
