#pragma once

#include "sema/module/behaviour/impl.h"
#include "sema/type/type.h"
#include <stddef.h>

typedef struct {
    SemaBehaviourKind kind;
    size_t idx;
    SemaType *self;
} SemaBehaviourTablePathElement;

typedef struct {
    SemaBehaviourTablePathElement *path;
    SemaType *generic;
} SemaBehaviourTablePath;

