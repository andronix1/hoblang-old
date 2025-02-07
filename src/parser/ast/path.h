#pragma once

#include "core/slice.h"
#include "sema/value.h"

typedef struct {
	Slice *segments;

	union {
		struct _SemaModule *module;
		struct _SemaScopeDecl *decl;
		struct _SemaType *type;
	};
} AstDeclPath;

typedef enum {
	AST_INNER_PATH_SEG_IDENT,
	AST_INNER_PATH_SEG_DEREF,
} AstInnerPathSegmentType;

typedef enum {
	SEMA_INNER_PATH_DEREF,
    SEMA_INNER_PATH_SLICE_LEN,
    SEMA_INNER_PATH_SLICE_RAW,
    SEMA_INNER_PATH_STRUCT_MEMBER,
} SemaInnerPathType;

typedef struct {
    size_t idx;
    struct _SemaType *of;
} SemaInnerPathStructMember;

typedef struct {
    SemaInnerPathType type;
    struct _SemaType *sema_type;
    union {
        struct _SemaType *deref_type;
        struct _SemaType *slice_type;
        SemaInnerPathStructMember struct_member;
    };
} SemaInnerPath;

typedef struct {
	AstInnerPathSegmentType type;
	union {
		Slice ident;
	};
    SemaInnerPath sema;
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
