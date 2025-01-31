#pragma once

#include "core/slice.h"

typedef enum {
    AST_PATH_SEGMENT_IDENT
} AstPathSegmentType;

typedef enum {
    SEMA_PATH_SEGMENT_MODULE,
    SEMA_PATH_SEGMENT_DECL,
    SEMA_PATH_SEGMENT_TYPE,
    SEMA_PATH_SEGMENT_STRUCT_MEMBER,
    SEMA_PATH_SEGMENT_SLICE_LENGTH,
    SEMA_PATH_SEGMENT_SLICE_PTR,
} SemaPathSegmentType;

typedef struct {
    size_t member_id;
    struct _SemaType *struct_type;
} SemaPathStructMember;

typedef struct {
    SemaPathSegmentType type;

    union {
        struct _SemaModule *module;
        struct _SemaType *slice_type;
        struct _SemaType *sema_type;
        struct _SemaScopeDecl *decl;
        SemaPathStructMember struct_member;
    };
} SemaPathSegment;

typedef struct {
    AstPathSegmentType type;
    union {
        Slice name;
    };
    SemaPathSegment sema;
} AstPathSegment;

typedef struct {
    AstPathSegment *segments;
} AstPath;