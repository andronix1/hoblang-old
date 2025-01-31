#pragma once

#include "core/slice.h"

typedef struct {
	Slice *segments;

	union {
		struct _SemaModule *module;
		struct _SemaScopeDecl *decl;
		struct _SemaType *type;
	};
} AstDeclPath;

typedef enum {
	AST_INNER_PATH_SEG_IDENT
} AstInnerPathSegmentType;

typedef struct {
	AstInnerPathSegmentType type;
	union {
		Slice ident;
	};
} AstInnerPathSegment;

typedef struct {
	AstInnerPathSegment *segments;
} AstInnerPath;

typedef enum {
    AST_PATH_SEGMENT_IDENT
} AstPathSegmentType;

typedef struct {
	AstDeclPath decl_path;
	AstInnerPath inner_path;
} AstPath;
