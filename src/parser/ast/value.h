#pragma once

typedef enum {
	AST_VALUE_IDENT,
	AST_VALUE_REF,
	AST_VALUE_DEREF
} AstValueType;

struct _SemaType;

typedef struct {
	struct _SemaType *sema_type;
	AstValueType type;
	union {
		Slice ident;
	};
} AstValueSegment;

typedef struct {
	struct _SemaType *sema_type;
	AstValueSegment *segments;
} AstValue;
